# client-server_system
Сервер:
Я вирішив викновати роботу за допомогою сокетів.
В цьому мені допоможе бібліотека Windows для роботи з мережою, їх є 2
версії. Я буду використовувати другу: “WinSock2.h” .
Перш ніж починати роботу з мережою, треба загрузити відповідну версію
біблотеки. Якщо цього не зробити, будь який визов мережевої функції
буде повертати помилку. Для загрузки бібліотеки викоростовується
функція WSAStartup. Ця функція приймає версію бібліотеки першим
параматером і посилання на wsaData другим.Одразу після спроби
загрузити бібліотеку, варто зробити перевірку. Якщо бібліотека
загрузилась коректно, функція поверне 0, інакше ми виводим помилку й
виходим з функції.
Далі необхідно заповнити інформацію про сокет. addr.sin_family -
сімейство протоколів, для інтернет протоколів вказуєтся AF_INET.
Щоб 2 комп&#39;ютера встановили з&#39;єднання, один з них повинен встановити
прослуховування на певному порту. socket(AF_INET, SOCK_STREAM, NULL)
Функція socket створює сокет і приймає 3 параметри. Перший - AF_INET
вказує на те, що буде використовуватися сімейство інтернет протоколів.
Другий- SOCK_STREAM вказує на протокол встановлючий з&#39;єднання, і
третій параметер нас зараз не потрібен, тому NULL.
Функція bind(sListen, (SOCKADDR*)&amp;addr, sizeof(addr)) втсановлює зв&#39;язує
сокет з адрессою та приймає наступні параметри: перший - попередньо
створенний сокет(sListen), другий - вказівник на структуру SOCKADDR,
третій- розмір структури SOCKADDR.
Функція listen(sListen, SOMAXCONN) прослуховує порт очікуючи
підключення з сторони клієнту. Приймає 2 параматри: перший - по якому
сокету прослоховувати, другий - максимальна кількість запитів очікуючих
обробки.
Для утримання декількох клієнтів, треба створити масив підключень,
кожне підключення записувати в масив і за відповідним індексом

створювати окремий потік обробки данних. Також варто пам&#39;ятати про те,
що треба створити змінну яка буде слідкувати за індексом з&#39;єднання
(Counter).
Функція CreateThread(NULL, NULL,
(LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL)
Створює новий потік в якому буде виконуватися функція ClientHandler з
переданним параметром i який відповідай за індекс ітерації
цикла for (int i = 0; i &lt; 100; i++)
Функції send і recv відправляють та отримують данні відповідно.
Аналогічно до масиву підключень, створюємо масив нікнеймів які
підключилися , та записуєм їх у цей масив.
В функції ClientHandler створюємо цикл який буде працювати доки
первірка result= recv(Connections[index], rec, sizeof(rec), NULL) не поверне
число менше або дорівнює 0. Доки result&gt;0 сервер очікує нікнейм
отримувача й повідомлення яке йому треба відправити. Коли ці данні
отримані, він починає перебирати всіх підключених клієнтів й шукати той
клієнт в якого такий самиій нікнейм як і нікнейм якому треба відправити
посилання. Далі він відправляє посилання відповідному користувачу.

Клієнт має нескінченний цикл while в main перед яким клієнт підключаєтся
до серверу, вводить свій нікнейм та відправляє його.
В циклі, клієнт спочатку вводить отримувача й повідомлення йому, потім їх
відправляє на сервер. Також клієнт створює додатковий потік, для
прийому повідомлень з серверу
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler,
NULL, NULL, NULL)
Функція ClientHandler() чекає на повідомлення, перевіряє, чи є воно
коректним і виводить його в консоль.
Для того щоб мати можливість відправляти великі повідомлення,
викорстовуєтся динамічне виділення пам&#39;яті. Але оскільки для того щоб
функція recv приймала повідомлення треба знати його розмір. Вводимо

змінну типу int в яку записуєм розмір посилання і відправляємо її перед
відправкою самого посилання.