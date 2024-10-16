#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <lm.h>
#pragma comment(lib, "Netapi32.lib")
using namespace std;



wchar_t* convertido(char* nombre){
    int caracteres_necesarios = MultiByteToWideChar(CP_ACP, 0, nombre, -1, NULL, 0);
    wchar_t* nombre_wchar = new wchar_t[caracteres_necesarios];
    MultiByteToWideChar(CP_ACP, 0, nombre, -1, nombre_wchar, caracteres_necesarios);
    return nombre_wchar;
}


int main() {
    //MessageBox(NULL, "Quieres guardar cambios?", "Título", MB_OK);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcess(NULL, 
                  (LPSTR)"notepad.exe",
                  NULL,
                  NULL,
                  FALSE,
                  0,
                  NULL,
                  NULL,
                  &si,
                  &pi);

    //Shellexecute
    //FindWindow

    // Información del sistema
    // Número de procesadores
    // Tipo de procesador
    // Arquitectura del sistema
    
    
    Sleep(8000);
    TerminateProcess(pi.hProcess, 0);

    // Obtener el método de la solicitud (GET o POST)
    const char* request_method = getenv("REQUEST_METHOD");
    char* query_string = nullptr;

    if (request_method != nullptr && strcmp(request_method, "GET") == 0) {
        // Si el método es GET, leer la cadena de consulta de la URL
        query_string = getenv("QUERY_STRING");
    } 
    else if (request_method != nullptr && strcmp(request_method, "POST") == 0) {
        // Si el método es POST, leer el contenido de la entrada estándar (stdin)
        const char* content_length_str = getenv("CONTENT_LENGTH");
        int content_length = 0;
        if (content_length_str != nullptr) {
            content_length = atoi(content_length_str); // Convertir a entero
        }
        query_string = new char[content_length + 1];  // Reservar espacio para el contenido POST
        cin.read(query_string, content_length);       // Leer desde stdin
        query_string[content_length] = '\0';          // Asegurarse de que sea una cadena terminada
    }


    // Inicializar variables
    char usuario[50] = "";
    char contra[50] = "";
    char orden[50] = "";

    if (query_string != nullptr) {
        // Parsear los parámetros
        char* param = strdup(query_string);  // Hacer una copia de la cadena de consulta
        char* token = strtok(param, "&");    // Separar los parámetros
        while (token != nullptr) {
            if (sscanf(token, "u=%49s", usuario) == 1) {  }
            if (sscanf(token, "c=%49s", contra) == 1) {  }
            if (sscanf(token, "orden=%49s", orden) == 1) {  }
            token = strtok(nullptr, "&");    // Continuar con el siguiente parámetro
        }
        free(param);  // Liberar la memoria
    }

    // Generar la respuesta HTTP
    cout << "Content-Type: text/plain\n\n";
    cout << "Nuevo usuario <b>" << usuario << "</b><br>";
    cout << "Contraseña <i>" << contra << "</i><br>";
    cout << "La orden es <i>" << orden << "</i><br>";
    cout << "<br>";


    string orden_string = string(orden);

    if(orden_string == "crea"){

        ////AÑADIR USUARIO

        USER_INFO_1 ui;
        DWORD dwLevel = 1;
        DWORD dwError = 0;
        

        // Configura los datos del nuevo usuario
        ui.usri1_name = (LPWSTR)convertido(usuario); // Nombre del usuario
        ui.usri1_password = (LPWSTR)convertido(contra); // Contraseña
        ui.usri1_priv = USER_PRIV_USER; // Privilegios del usuario (USER_PRIV_USER para usuario estándar)
        ui.usri1_home_dir = NULL;
        ui.usri1_comment = NULL;
        ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD; // El usuario debe usar script y la contraseña no expira
        ui.usri1_script_path = NULL;

        // Llama a NetUserAdd para crear el usuario
        NET_API_STATUS nStatus = NetUserAdd(NULL, dwLevel, (LPBYTE)&ui, &dwError);

        if (nStatus == NERR_Success) {
            cout << "<span class='text-success'>";
            cout << "Usuario creado con éxito<br>";
            cout << "</span>";
        } else {
            cout << "<span class='text-danger'>";
            cout << "Error al crear el usuario.<br>Código de error: " << nStatus << "<br>";
            cout << "</span>";
        }
    }


    if(orden_string == "elimina"){
        USER_INFO_1 ui;
        DWORD dwLevel = 1;
        DWORD dwError = 0;

        LPWSTR usElim = (LPWSTR)convertido(usuario); // Nombre del usuario

        // Llama a NetUserAdd para crear el usuario
        NET_API_STATUS nStatus = NetUserDel(NULL, usElim);

        if (nStatus == NERR_Success) {
            cout << "<span class='text-success'>";
            cout << "Usuario eliminado con éxito<br>";
            cout << "</span>";
        } else {
            cout << "<span class='text-danger'>";
            cout << "Error al eliminar el usuario.<br>Código de error: " << nStatus << "<br>";
            cout << "</span>";
        }
    }


    if(orden_string == "edita"){
        USER_INFO_1003 ui;
        DWORD dwLevel = 1003;
        DWORD dwError = 0;

        LPWSTR usEdit = (LPWSTR)convertido(usuario); // Nombre del usuario
        ui.usri1003_password = (LPWSTR)convertido(contra);

        // Llama a NetUserAdd para crear el usuario
        NET_API_STATUS nStatus = NetUserSetInfo(NULL, usEdit, dwLevel, (LPBYTE)&ui, &dwError);


        if (nStatus == NERR_Success) {
            cout << "<span class='text-success'>";
            cout << "Contraseña cambiada con éxito<br>";
            cout << "</span>";
        } else {
            cout << "<span class='text-danger'>";
            cout << "Error al cambiar la contraseña.<br>Código de error: " << nStatus << "<br>";
            cout << "</span>";
        }
    }



    return 0;
}