// #include <iostream>
// #include <string>
// #include "MysqlConnectPoll.h"
// #include <chrono>
// int main()
// {
//     MysqlConnectPoll *poll = MysqlConnectPoll::getMysqlConnectPoll();
//     std::cout << poll->getPollSize() <<"\n";
//     std::shared_ptr<MysqlConnect> arr[10];
//     for(int i=0;i<10;i++)
//     {
//         arr[i] = poll->getConnect();
//     }

//     std::cout << poll->getPollSize() <<"\n";

//     for(int i=0;i<10;i++)
//     {
//         auto connect = arr[i];
//         if (connect->query("select * from user"))
//         {
//             while (connect->next())
//             {
//                 std::cout << connect->index(0) << "\t" << connect->index(1) <<'\n';
//             }

//         }
//         connect.reset();
//     }

//     std::this_thread::sleep_for(std::chrono::seconds(4));
//     std::cout << poll->getPollSize();
//     return 0;
// }