
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> // бібліотека для роботи з мережею
#include <iostream>  //бібліотека вводу й виводу
#include <string>

#pragma warning(disable: 4996)      //відключення попередження

SOCKET Connections[100];           //масив підключень
int Counter = 0;                    //ітератор
std::string Names[100];             //масив нікнеймов
void ClientHandler(int index) {         //функція роботи з клієнтом
    int msg_size;
    char rec[256];                      //нікнейм, кому треба відправити повідомлення
    int result;                         //треба для перевірки коректності повідомлення
    int k=1;                            //для можливості вийти з нескінечнного циклу
    while (k==1) {
        char* msg = new char;
       
        result= recv(Connections[index], rec, sizeof(rec), NULL);       //отримали нікнейм, кому треба відправити повідомлення
        if (result > 0) {                                               //перевірка коректності повідомлення
            recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);//отримали розмір повідомлення
            msg = new char[msg_size + 1];
            msg[msg_size] = '\0';
            recv(Connections[index], msg, msg_size, NULL);                //отримали повідомлення

            for (int i = 0; i < Counter; i++) {                             //цикл пошуку потрібного отримувача
                if (Names[i] == rec) {
                    send(Connections[i], (char*)&msg_size, sizeof(int), NULL); //відправлення отримувачу розміру повідомлення
                    send(Connections[i], msg, msg_size, NULL);                  //віддправили повідомлення отримувачу
                }
                
            }
            k = 1;
        }
        else {
            std::cout <<Names[index]+" disconected" << std::endl;                           //повідомлення, про те що клієнт відключився

            k = 0;
        }
        delete[] msg;                                                               //звільнили пам'ять
    }
}

int main(int argc, char* argv[]) {
    
    WSAData wsaData;                                //створення бібліотеки
    WORD DLLVersion = MAKEWORD(2, 1);               //запит на подрібну версію
    if (WSAStartup(DLLVersion, &wsaData) != 0) {    //перевірка встановлення бібліотеки
        std::cout << "Error" << std::endl;
        exit(1);                                    //вихід з функції
    }
                                                     
    SOCKADDR_IN addr;                                  //інформація про адрессу сокету
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");     //локалхост
    addr.sin_port = htons(1111);                        //порт
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //створення сокету 
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));      //прив'язка адреса сокету
    listen(sListen, SOMAXCONN);                         //очікування з'єднання з сторони клієнту

    std::cout << "Server ready!\n";                     //повідомлення про те, що сервер працює коректно
    SOCKET newConnection;                               //створення сокету для нового підключення
    for (int i = 0; i < 100; i++) {                     //цикл для всіх підключень
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); //утримання з'єднання

        if (newConnection == 0) {                                   //перевірка успішності підключення
            std::cout << "Error #2\n";
        }
        else {
            char Nick[256];                                         //оголошення нікнейму
            
            std::string msg = "Welcome to channel ";                //привітання від серверу
            
            recv(newConnection, Nick, sizeof(Nick), NULL);          //отримуємо нікнейм від клієнта
            std::cout << Nick << " Connected!\n";                   //виводимо в консолі серверу що він під'єднався
            
            msg = msg+Nick+"!";                                     //доповнюємо привітання нікнеймом
            int msg_size = msg.size();
            
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);   //відправили розмір повідомлення
            send(newConnection, msg.c_str(), msg_size, NULL);           //відправили повідомлення
            Connections[i] = newConnection;                             //присвоїли підключення масиву підключень
            
            Names[i] = Nick;                                            //присвоїли нікнейм масиву нікнеймів
            
            Counter++;
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL); //створили додатковий потік
        }
    }


    system("pause");
    return 0;
}