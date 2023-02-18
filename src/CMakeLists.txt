kcoreaddons_add_plugin(kwin4_effect_soft_open_close INSTALL_NAMESPACE "kwin/effects/plugins")

target_sources(kwin4_effect_soft_open_close PRIVATE
    main.cpp
    softopencloseeffect.cpp
)

target_link_libraries(kwin4_effect_soft_open_close
    Qt5::Core
    Qt5::Gui

    KF5::CoreAddons
    KF5::WindowSystem

    KWinEffects::kwineffects
)
