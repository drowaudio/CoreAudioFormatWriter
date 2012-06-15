# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  TARGET := AudioWriter
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  TARGET := AudioWriter
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/MainWindow_499ac812.o \
  $(OBJDIR)/CoreAudioFormat_a85923af.o \
  $(OBJDIR)/AudioDemoTabComponent_47aba6c.o \
  $(OBJDIR)/AudioDemoSetupPage_a517faf2.o \
  $(OBJDIR)/AudioDemoRecordPage_7ccc13c4.o \
  $(OBJDIR)/juce_audio_basics_2cb80bf0.o \
  $(OBJDIR)/juce_audio_devices_649024ae.o \
  $(OBJDIR)/juce_audio_formats_93116e4e.o \
  $(OBJDIR)/juce_audio_processors_ee99f5a8.o \
  $(OBJDIR)/juce_audio_utils_e1ba436e.o \
  $(OBJDIR)/juce_core_f4b674d2.o \
  $(OBJDIR)/juce_data_structures_2af81f0e.o \
  $(OBJDIR)/juce_events_357e2846.o \
  $(OBJDIR)/juce_graphics_4da974aa.o \
  $(OBJDIR)/juce_gui_basics_baae7726.o \
  $(OBJDIR)/juce_gui_extra_ac703a2e.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking AudioWriter
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning AudioWriter
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CoreAudioFormat_a85923af.o: ../../Source/CoreAudioFormat.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CoreAudioFormat.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioDemoTabComponent_47aba6c.o: ../../Source/AudioDemo/AudioDemoTabComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioDemoTabComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioDemoSetupPage_a517faf2.o: ../../Source/AudioDemo/AudioDemoSetupPage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioDemoSetupPage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioDemoRecordPage_7ccc13c4.o: ../../Source/AudioDemo/AudioDemoRecordPage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioDemoRecordPage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_2cb80bf0.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_649024ae.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_93116e4e.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_ee99f5a8.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_e1ba436e.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_f4b674d2.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_2af81f0e.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_357e2846.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_4da974aa.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_baae7726.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_ac703a2e.o: ../../../../Documents/Developement/juce_source/juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
