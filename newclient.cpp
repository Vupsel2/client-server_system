#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#pragma warning(disable: 4996)          //відключення попередження

SOCKET Connection;


void ClientHandler() {                                          //функція роботи з сервером                                    
    int msg_size;
    int result;
    int k = 1;
    while (k==1) {                                              //нескінченний цикл
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);  //отримали розмір повідомлення
        
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
       
        result =recv(Connection, msg, msg_size, NULL);          //отримали повідомлення і зробили перевірку на його коректність
        if (result > 0) {
           std::cout << msg << std::endl;                       //вивели повідомлення
           k = 1;
        }
        else if (result == 0) {
            std::cout << "Conecction closed" << std::endl;
            exit(1);
            k = 0;
        }
        else {
            std::cout << "Server closed" << std::endl;
            k = 0;
            exit(1);
        }
       
        
        delete[] msg;                                          //звільнили пам'ять
    }
}

int main(int argc, char* argv[]) {
    
    system("color 2");                      //зміна кольору тексту в клієнті
   
    
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);                               //аналогічно, яка і в сервері, створюємо сокет
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);                        //підключення до серверу
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {         //перевірка успішності
        std::cout << "Error: failed connect to server.\n";
        return 1;                                                          //виход якщо не вдалося
    }

    

    std::cout << "Connected!\nPrint'exit' in 'Your massage'to Disconect\n";   //якщо вдалося, повідомлення про це в консолі
    char Nick1[256];                                                        //ініціалізація никнейму
    
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL); //створення потоку з функцією роботи з сервером
    std::cout << "Enter your nickname: ";                                               //запит про нікнейм
    std::cin.getline(Nick1, sizeof(Nick1));                                             //отримали нікнейм в вигляді char
    send(Connection, Nick1, sizeof(Nick1), NULL);                                       //відправили нікнейм на сервер
    Sleep(10);                                                                          //пауза для відповіді серверу
    std::string msg1;                                                                   //ініціалізація повідомлення

    std::string s;
    s += Nick1;                                                                         //перетворення нікнейму в рядок

    char rec1[256];                                                                     //ініціалізація нікнейму отримувача
 
    
    s += ": ";                                                                          //космітичне доповнення нікнейму
    
    while (true) {                                                                      //функція відправлення повідомлення й нікнейму отримувача серверу
        std::cout << "Enter  receiver name: ";
        std::cin.getline(rec1, sizeof(rec1));                                           //отримали нікнекйм отримувача
        
        std::cout << "Your massage: ";                                                  
        std::getline(std::cin, msg1);                                                   //отримали повідомлення
        if (msg1 == "exit") { std::cout << "\nDisconected"; exit(1); }                  //створили умову для відключення від серверу
        msg1 = "\n"+s + msg1;                                                           //поєднали нікнейм відпраника з його повідомленням
        int msg_size = msg1.size();                                                     //рахуєм розмір повідомлення
        send(Connection, rec1, sizeof(rec1), NULL);                                     //відправили нікнейм отримувача
        send(Connection, (char*)&msg_size, sizeof(int), NULL);                          //відправили розмір повідомлення
        send(Connection, msg1.c_str(), msg_size, NULL);                                 //відправили повідомлення
        Sleep(10);                                                                      //час для відповіді серверу

        
    }

    system("pause");
    return 0;
}