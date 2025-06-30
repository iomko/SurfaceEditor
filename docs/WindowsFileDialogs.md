# WindowsFileDialogs
The WindowsFileDialogs class provides static methods for displaying native Windows file dialog boxes. These dialogs are tightly integrated with the application's GLFW window using the Win32 API.

###### Public Static Methods
`std::string openFile(const char* filter)`
Opens a native Open File dialog window using the provided filter (e.g., "Text Files\0*.txt\0All Files\0*.*\0"). If the user selects a file, the method returns its full path as a std::string. If the user cancels the dialog, an empty string is returned.

`std::string saveFile(const char* filter)`
Opens a native Save File dialog window using the provided filter string. If the user confirms the dialog, the chosen file path is returned. Otherwise, the method returns an empty string.