/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "softopencloseeffect.h"

namespace KWin
{

static const qreal s_scale = 0.95;
static const int s_duration = 250;

static const QSet<QString> s_blacklist {
    QStringLiteral("ksmserver ksmserver"),
    QStringLiteral("ksmserver-logout-greeter ksmserver-logout-greeter"),
    QStringLiteral("ksplashqml ksplashqml"),
};

SoftOpenCloseEffect::SoftOpenCloseEffect()
{
    m_openEasingCurve.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
    m_closeEasingCurve.addCubicBezierSegment(QPointF(1, 0), QPointF(1, 1), QPointF(1, 1));

    connect(effects, &EffectsHandler::windowAdded, this, &SoftOpenCloseEffect::slotWindowAdded);
    connect(effects, &EffectsHandler::windowClosed, this, &SoftOpenCloseEffect::slotWindowClosed);
    connect(effects, &EffectsHandler::windowDataChanged, this, &SoftOpenCloseEffect::slotWindowDataChanged);
}

void SoftOpenCloseEffect::slotWindowAdded(EffectWindow *window)
{
    if (effects->activeFullScreenEffect()) {
        return;
    }

    if (!isSoftOpenCloseWindow(window)) {
        return;
    }

    if (!window->isVisible()) {
        return;
    }

    const void *addGrab = window->data(WindowAddedGrabRole).value<void *>();
    if (addGrab && addGrab != this) {
        return;
    }

    window->setData(WindowAddedGrabRole, QVariant::fromValue(static_cast<void *>(this)));

    animate(window, Attribute::Opacity, 0, animationTime(s_duration), FPx2(1, 1), m_openEasingCurve, 0, FPx2(0, 0));
    animate(window, Attribute::Scale, 0, animationTime(s_duration), FPx2(1, 1), m_openEasingCurve, 0, FPx2(s_scale, s_scale));
}

void SoftOpenCloseEffect::slotWindowClosed(EffectWindow *window)
{
    if (effects->activeFullScreenEffect()) {
        return;
    }

    if (!isSoftOpenCloseWindow(window)) {
        return;
    }

    if (!window->isVisible() || window->skipsCloseAnimation()) {
        return;
    }

    const void *closeGrab = window->data(WindowClosedGrabRole).value<void *>();
    if (closeGrab && closeGrab != this) {
        return;
    }

    window->setData(WindowClosedGrabRole, QVariant::fromValue(static_cast<void *>(this)));

    animate(window, Attribute::Opacity, 0, animationTime(s_duration), FPx2(0, 0), m_closeEasingCurve);
    animate(window, Attribute::Scale, 0, animationTime(s_duration), FPx2(s_scale, s_scale), m_closeEasingCurve);
}

void SoftOpenCloseEffect::slotWindowDataChanged(EffectWindow *window, int role)
{
    if (role != WindowAddedGrabRole && role != WindowClosedGrabRole) {
        return;
    }

    if (window->data(role).value<void *>() == this) {
        return;
    }
}

bool SoftOpenCloseEffect::isSoftOpenCloseWindow(EffectWindow *window) const
{
    // We don't want to animate most of plasmashell's windows, yet, some
    // of them we want to, for example, Task Manager Settings window.
    // The problem is that all those window share single window class.
    // So, the only way to decide whether a window should be animated is
    // to use a heuristic: if a window has decoration, then it's most
    // likely a dialog or a settings window so we have to animate it.
    if (window->windowClass() == QLatin1String("plasmashell plasmashell")
        || window->windowClass() == QLatin1String("plasmashell org.kde.plasmashell")) {
        return window->hasDecoration();
    }

    if (s_blacklist.contains(window->windowClass())) {
        return false;
    }

    if (window->hasDecoration()) {
        return true;
    }

    // Don't animate combobox popups, tooltips, popup menus, etc.
    if (window->isPopupWindow()) {
        return false;
    }

    // Don't animate the outline and the screenlocker as it looks bad.
    if (window->isLockScreen() || window->isOutline()) {
        return false;
    }

    // Override-redirect windows are usually used for user interface
    // concepts that are not expected to be animated by this effect.
    if (window->isX11Client() && !window->isManaged()) {
        return false;
    }

    return window->isNormalWindow() || window->isDialog();
}

} // namespace KWin
