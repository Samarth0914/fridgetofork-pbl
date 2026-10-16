CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Werror

SAMARTH_DIR := 1_Samarth_Aggarwal
GOVIND_DIR  := 2_Govind_Chhabra
AKANSHA_DIR := 3_Akansha_Khandelwal
AMOGH_DIR   := 4_Amogh_Sharma

SRCS := $(SAMARTH_DIR)/Ingredient.cpp \
        $(SAMARTH_DIR)/Recipe.cpp \
        $(AMOGH_DIR)/JsonDatasetLoader.cpp \
        $(AKANSHA_DIR)/MatchingAndNearMatch.cpp \
        $(GOVIND_DIR)/RankingEngine.cpp \
        $(SAMARTH_DIR)/main.cpp

TARGET := fridgetofork_demo

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) Quick_Single_File_Version/fridgetofork_demo
