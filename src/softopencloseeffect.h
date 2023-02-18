/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <kwinanimationeffect.h>

namespace KWin
{

class SoftOpenCloseEffect : public AnimationEffect
{
    Q_OBJECT

public:
    SoftOpenCloseEffect();

    int requestedEffectChainPosition() const override;

private Q_SLOTS:
    void slotWindowAdded(EffectWindow *window);
    void slotWindowClosed(EffectWindow *window);
    void slotWindowDataChanged(EffectWindow *window, int role);

private:
    bool isSoftOpenCloseWindow(EffectWindow *w) const;

    QEasingCurve m_openEasingCurve;
    QEasingCurve m_closeEasingCurve;
};

inline int SoftOpenCloseEffect::requestedEffectChainPosition() const
{
    return 50;
}

} // namespace KWin
