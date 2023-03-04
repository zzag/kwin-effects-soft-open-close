kcoreaddons_add_plugin(kwin4_effect_soft_open_close INSTALL_NAMESPACE "kwin/effects/plugins")

target_sources(kwin4_effect_soft_open_close PRIVATE
    main.cpp
    softopencloseeffect.cpp
)

target_link_libraries(kwin4_effect_soft_open_close
    Qt6::Core
    Qt6::Gui

    KF6::CoreAddons
    KF6::WindowSystem

    KWinEffects::kwineffects
)
