/*
 *  Copyright (c) 2009-2010 Lukáš Tvrdý <lukast.dev@gmail.com>
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

#include "kis_dyna_paintop_settings.h"
#include <kis_paint_action_type_option.h>
#include <kis_airbrush_option.h>

struct KisDynaPaintOpSettings::Private
{
    QList<KisUniformPaintOpPropertyWSP> uniformProperties;
};

KisDynaPaintOpSettings::KisDynaPaintOpSettings()
    : m_d(new Private)
{
}

KisDynaPaintOpSettings::~KisDynaPaintOpSettings()
{
}

bool KisDynaPaintOpSettings::paintIncremental()
{
    return (enumPaintActionType)getInt("PaintOpAction", WASH) == BUILDUP;
}

bool KisDynaPaintOpSettings::isAirbrushing() const
{
    return getBool(AIRBRUSH_ENABLED);
}

int KisDynaPaintOpSettings::rate() const
{
    return getInt(AIRBRUSH_RATE);
}


#include <brushengine/kis_slider_based_paintop_property.h>
#include <brushengine/kis_combo_based_paintop_property.h>
#include "kis_paintop_preset.h"
#include "kis_paintop_settings_update_proxy.h"
#include "kis_dynaop_option.h"
#include "kis_standard_uniform_properties_factory.h"


QList<KisUniformPaintOpPropertySP> KisDynaPaintOpSettings::uniformProperties()
{
    QList<KisUniformPaintOpPropertySP> props =
        listWeakToStrong(m_d->uniformProperties);

    if (props.isEmpty()) {
        {
            KisIntSliderBasedPaintOpPropertyCallback *prop =
                new KisIntSliderBasedPaintOpPropertyCallback(
                    KisIntSliderBasedPaintOpPropertyCallback::Int,
                    "dyna_diameter",
                    i18n("Diameter"),
                    this, 0);

            prop->setRange(0, 1000);
            prop->setSingleStep(1);
            prop->setSuffix(i18n(" px"));

            prop->setReadCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());

                    prop->setValue(option.dyna_diameter);
                });
            prop->setWriteCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    option.dyna_diameter = prop->value().toInt();
                    option.writeOptionSetting(prop->settings().data());
                });

            QObject::connect(preset()->updateProxy(), SIGNAL(sigSettingsChanged()), prop, SLOT(requestReadValue()));
            prop->requestReadValue();
            props << toQShared(prop);
        }
        {
            KisIntSliderBasedPaintOpPropertyCallback *prop =
                new KisIntSliderBasedPaintOpPropertyCallback(
                    KisIntSliderBasedPaintOpPropertyCallback::Int,
                    "dyna_angle",
                    i18n("Angle"),
                    this, 0);

            const QString degree = QChar(Qt::Key_degree);
            prop->setRange(0, 360);
            prop->setSingleStep(1);
            prop->setSuffix(degree);

            prop->setReadCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());

                    prop->setValue(int(option.dyna_angle));
                });
            prop->setWriteCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    option.dyna_angle = prop->value().toInt();
                    option.writeOptionSetting(prop->settings().data());
                });
            prop->setIsVisibleCallback(
                [](const KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    return option.dyna_use_fixed_angle;
                });

            QObject::connect(preset()->updateProxy(), SIGNAL(sigSettingsChanged()), prop, SLOT(requestReadValue()));
            prop->requestReadValue();
            props << toQShared(prop);
        }
        {
            KisDoubleSliderBasedPaintOpPropertyCallback *prop =
                new KisDoubleSliderBasedPaintOpPropertyCallback(
                    KisDoubleSliderBasedPaintOpPropertyCallback::Double,
                    "dyna_mass",
                    i18n("Mass"),
                    this, 0);

            prop->setRange(0.01, 3);
            prop->setSingleStep(0.01);
            prop->setDecimals(2);
            prop->setExponentRatio(3);

            prop->setReadCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    prop->setValue(option.dyna_mass);
                });
            prop->setWriteCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    option.dyna_mass = prop->value().toReal();
                    option.writeOptionSetting(prop->settings().data());
                });

            QObject::connect(preset()->updateProxy(), SIGNAL(sigSettingsChanged()), prop, SLOT(requestReadValue()));
            prop->requestReadValue();
            props << toQShared(prop);
        }
        {
            KisDoubleSliderBasedPaintOpPropertyCallback *prop =
                new KisDoubleSliderBasedPaintOpPropertyCallback(
                    KisDoubleSliderBasedPaintOpPropertyCallback::Double,
                    "dyna_drag",
                    i18n("Drag"),
                    this, 0);

            prop->setRange(0, 0.99);
            prop->setSingleStep(0.01);
            prop->setDecimals(2);
            prop->setExponentRatio(3);

            prop->setReadCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    prop->setValue(option.dyna_drag);
                });
            prop->setWriteCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    option.dyna_drag = prop->value().toReal();
                    option.writeOptionSetting(prop->settings().data());
                });

            QObject::connect(preset()->updateProxy(), SIGNAL(sigSettingsChanged()), prop, SLOT(requestReadValue()));
            prop->requestReadValue();
            props << toQShared(prop);
        }
        {
            KisComboBasedPaintOpPropertyCallback *prop =
                new KisComboBasedPaintOpPropertyCallback(
                    "dyna_shape",
                    i18n("Shape"),
                    this, 0);

            QList<QString> shapes;
            shapes << i18n("Circle");
            shapes << i18n("Polygon");
            shapes << i18n("Wire");
            shapes << i18n("Lines");

            prop->setItems(shapes);

            prop->setReadCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());

                    prop->setValue(option.dyna_action);
                });
            prop->setWriteCallback(
                [](KisUniformPaintOpProperty *prop) {
                    DynaOption option;
                    option.readOptionSetting(prop->settings().data());
                    option.dyna_action = prop->value().toInt();
                    option.writeOptionSetting(prop->settings().data());
                });

            QObject::connect(preset()->updateProxy(), SIGNAL(sigSettingsChanged()), prop, SLOT(requestReadValue()));
            prop->requestReadValue();
            props << toQShared(prop);
        }
    }

    {
        using namespace KisStandardUniformPropertiesFactory;

        Q_FOREACH (KisUniformPaintOpPropertySP prop, KisPaintOpSettings::uniformProperties()) {
            if (prop->id() == opacity.id() ||
                prop->id() == flow.id()) {

                props.prepend(prop);
            }
        }
    }

    return props;
}
