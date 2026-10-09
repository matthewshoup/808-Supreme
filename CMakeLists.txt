cmake_minimum_required(VERSION 3.22)
project(808Supreme VERSION 0.2.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.4)
FetchContent_MakeAvailable(JUCE)
juce_add_plugin(808Supreme COMPANY_NAME "Matthew Shoup" BUNDLE_ID "com.matthewshoup.808supreme"
    PLUGIN_MANUFACTURER_CODE MShp PLUGIN_CODE ES08 FORMATS VST3 Standalone PRODUCT_NAME "808 Supreme"
    IS_SYNTH TRUE NEEDS_MIDI_INPUT TRUE NEEDS_MIDI_OUTPUT FALSE IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE COPY_PLUGIN_AFTER_BUILD FALSE)
target_sources(808Supreme PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp Source/DSP/Kick808.cpp Source/DSP/Drum808.cpp)
target_compile_definitions(808Supreme PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(808Supreme PRIVATE juce::juce_audio_utils juce::juce_dsp PUBLIC
    juce::juce_recommended_config_flags juce::juce_recommended_lto_flags juce::juce_recommended_warning_flags)
