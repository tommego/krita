/*
 *  Copyright (c) 2016 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "kis_stabilized_events_sampler.h"

#include <QList>
#include <QElapsedTimer>
#include <QtMath>

#include "kis_paint_information.h"


struct KisStabilizedEventsSampler::Private
{
    Private(int _sampleTime) : sampleTime(_sampleTime), elapsedTimeOverride(0) {}

    std::function<void(const KisPaintInformation &)> paintLine;
    QElapsedTimer lastPaintTime;
    QList<KisPaintInformation> realEvents;
    int sampleTime;
    int elapsedTimeOverride;

    KisPaintInformation lastPaintInformation;
};

KisStabilizedEventsSampler::KisStabilizedEventsSampler(int sampleTime)
    : m_d(new Private(sampleTime))
{
}

KisStabilizedEventsSampler::~KisStabilizedEventsSampler()
{
}

void KisStabilizedEventsSampler::setLineFunction(std::function<void(const KisPaintInformation &)> func)
{
    m_d->paintLine = func;
}

void KisStabilizedEventsSampler::clear()
{
    if (!m_d->realEvents.isEmpty()) {
        m_d->lastPaintInformation = m_d->realEvents.last();
    }

    m_d->realEvents.clear();
    m_d->lastPaintTime.start();
}

void KisStabilizedEventsSampler::addEvent(const KisPaintInformation &pi)
{
    if (!m_d->lastPaintTime.isValid()) {
        m_d->lastPaintTime.start();
    }

    m_d->realEvents.append(pi);
}

void KisStabilizedEventsSampler::addFinishingEvent(int numSamples)
{
    clear();

    m_d->elapsedTimeOverride = numSamples;
    m_d->realEvents.append(m_d->lastPaintInformation);
}

void KisStabilizedEventsSampler::processAllEvents()
{
    const int elapsed = m_d->lastPaintTime.restart();

    const qreal alpha = qreal(m_d->realEvents.size()) / elapsed;

    for (int i = 0; i < elapsed; i += m_d->sampleTime) {
        const int k = qFloor(alpha * i);

        m_d->paintLine(m_d->realEvents[k]);
    }
}

const KisPaintInformation& KisStabilizedEventsSampler::iterator::dereference() const
{
    const int k = qFloor(m_alpha * m_index);
    return k < m_sampler->m_d->realEvents.size() ?
        m_sampler->m_d->realEvents[k] : m_sampler->m_d->lastPaintInformation;
}

std::pair<KisStabilizedEventsSampler::iterator, KisStabilizedEventsSampler::iterator>
KisStabilizedEventsSampler::range() const
{
    const int elapsed = (m_d->lastPaintTime.restart() + m_d->elapsedTimeOverride) / m_d->sampleTime;
    const qreal alpha = qreal(m_d->realEvents.size()) / elapsed;

    m_d->elapsedTimeOverride = 0;

    return std::make_pair(iterator(this, 0, alpha),
                          iterator(this, elapsed, alpha));
}


