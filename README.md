# git-graph

##Enviroment Set Up
Python 3.4+ with *requests*
Mysql server 5.7 with python/c++ connector

Connector Guides:
python : https://dev.mysql.com/doc/connector-python/en/
c++: https://dev.mysql.com/doc/connector-cpp/en/ 

Newman's network data and wheel at:http://www-personal.umich.edu/~mejn/netdata/ Considering using gephi to present graph data

**About C++ Connecoters:** 
I got some trouble when using the Binary distributions which official website provided, the lib is pre-compiled with gcc, the compile using that lib is successfull while the program crash unreasonably when it calls driver->connect() method.I searched the interet but failed to figure out solution.So i turned to MSVC. 
(I'll be very appreciate if some can share how to solve that problem.:)


I'm currently using Visual Studio 2015.And the doc official website provided was a little outdated.

###C++ Connecotr For Mysql Building
1. clone current source code
    ```
    $ git clone https://github.com/mysql/mysql-connector-cpp
    $ cd ./mysql-connetor-cpp
    $ git checkout 1.1.8
    ``` 
 
2.  cmake
    ```
    $ mkdir ./build
    $ cmake .. -G "Visual Studio 14 2015 Win64" -DMYSQL_DIR="C:\Program Files\MySQL\MySQL Server 5.7" -DBOOST_ROOT="C:\Program Files (x86)\Dev-Cpp\MinGW64\boost_1_63_0"
    ```
Then we get .sln solution file for x64
You can also use -G "Visual Studio 14 2015" to generate default x86 version
But it should accordinate with your Mysql version(x86 or x64)

3. open MYSQLCPPCONN.sln with VS 
    Problems I encounterd solved in following link
http://stackoverflow.com/questions/33114535/timespec-struct-type-error-c2011
http://riccardotramma.com/2015/01/mysql-c-with-mysql-connector/


    



