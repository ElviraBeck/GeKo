set(imGUI_DOWNLOAD_PATH "${DEPENDENCIES_PATH}/Imgui")
set(imGUI_URL "https://raw.githubusercontent.com/ocornut/imgui/master")
	
if (NOT EXISTS "${imGUI_DOWNLOAD_PATH}")
	file(DOWNLOAD  "${imGUI_URL}/imgui.cpp" 	"${imGUI_DOWNLOAD_PATH}/imgui.cpp")
	file(DOWNLOAD  "${imGUI_URL}/imgui.h" 		"${imGUI_DOWNLOAD_PATH}/imgui.h")
	file(DOWNLOAD  "${imGUI_URL}/imconfig.h" 	"${imGUI_DOWNLOAD_PATH}/imconfig.h")
	file(DOWNLOAD  "${imGUI_URL}/stb_rect_pack.h" 	"${imGUI_DOWNLOAD_PATH}/stb_rect_pack.h")
	file(DOWNLOAD  "${imGUI_URL}/stb_textedit.h" 	"${imGUI_DOWNLOAD_PATH}/stb_textedit.h")
	file(DOWNLOAD  "${imGUI_URL}/stb_truetype.h" 	"${imGUI_DOWNLOAD_PATH}/stb_truetype.h")
endif()
	
set(IMGUI_INCLUDE_PATH "${DEPENDENCIES_PATH}/Imgui/")

