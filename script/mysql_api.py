import mysql.connector
from mysql.connector import errorcode
from mysql.connector import errors

config={
    'user':"root",
    'password':'li83916386',
    'host':'127.0.0.1',
    'database':'git',   
}

class data():

    add_user_info = ("INSERT INTO userinfo"
        "(id,login,avatar_url,name,email,location,bio,public_repos,public_gists,followers,following,created_at)"
        "VALUES (\"%(id)s\",\"%(login)s\",\"%(avatar_url)s\",\"%(name)s\",\"%(email)s\",\"%(location)s\",\"%(bio)s\",\"%(public_repos)s\""
        ",\"%(public_gists)s\",\"%(followers)s\",\"%(following)s\",\"%(created_at)s\")"
        )
    add_following = ("INSERT INTO following"
        "(userid,followingid) "
        "VALUES (\"%(userid)s\",\"%(followingid)s\")")
    add_follower  = ("INSERT INTO follower"
        "(userid,followerid) "
        "VALUES (\"%(userid)s\",\"%(followerid)s\")")
    find_user = ("SELECT login,id FROM userinfo where login='%s'")
    TYPE = ('userinfo','following','follower')

    def __init__(self,config=config):
        try:
            self.cnx = mysql.connector.connect(**config)
            self.cursor = self.cnx.cursor(buffered=True)
        except mysql.connector.Error as err:
            if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
                print("Something is wrong with your user name or password")
            elif err.errno == errorcode.ER_BAD_DB_ERROR:
                print("Database does not exist")
            else:
                print(err)
        else:
            print('Connection to mysql success!')

    def insert(self,type,**data):
        if type not in self.TYPE:
            return false
        try:
            if type == 'userinfo':
                self.cursor.execute((self.add_user_info%data).replace("'","''"))
            #Here replace method is used to convert character ' in words like "i'm" to "i''m" which is right escape character for mysql SYNTAX
            elif type == 'following':
                self.cursor.execute((self.add_following%data).replace("'","''"))
            elif type == 'follower':
                self.cursor.execute((self.add_follower%data).replace("'","''"))
        except errors.IntegrityError: 
            print('username exist!')
        else:
            self.cnx.commit()

    def recorded(self,username):
        self.cursor.execute(self.find_user%username)
        return bool(self.cursor.fetchall())

    def delete(self,):
        pass
    def clear(self,):
        pass

def mysql_init():
    try:
        cnx = mysql.connector.connect(**config)
    except mysql.connector.Error as err:
      if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print("Something is wrong with your user name or password")
      elif err.errno == errorcode.ER_BAD_DB_ERROR:
        print("Database does not exist")
      else:
        print(err)
#    else:
#      cnx.close()

    TABLES={}
#关于这个SQL语句比较坑..首先里面 字段名不用加引号 最后那个不要多加逗号，不像字典 多一个逗号它就认为多一个字段
    TABLES['userinfo'] =(
        "CREATE TABLE userinfo ("
        " id varchar(20) NOT NULL,"
        " login varchar(50) NOT NULL,"
        " avatar_url varchar(100),"
        " name varchar(50),"
        " email varchar(100),"
        " location varchar(200),"
        " bio varchar(300),"
        " public_repos int(5),"
        " public_gists int(5),"
        " followers int(5),"
        " following int(5),"
        " created_at date,"
        " primary key(id)"
        ")ENGINE=InnoDB")

    TABLES['following']=(
        "CREATE TABLE following ( "
        " userid varchar(50) NOT NULL, "
        " followingid varchar(50) NOT NULL"
        ")ENGINE=InnoDB" )

    TABLES['follower']=(
        "CREATE TABLE follower ("
        " userid varchar(50) NOT NULL,"
        " followerid varchar(50) NOT NULL"
        ")ENGINE=InnoDB" )

    cursor = cnx.cursor()
    for name, ddl in TABLES.items():
        try:
            print("Creating table {}: ".format(name), end='')
            cursor.execute(ddl)
        except mysql.connector.Error as err:
            if err.errno == errorcode.ER_TABLE_EXISTS_ERROR:
                print("already exists.")
            else:
                print(err.msg)
        else:
            print("OK")

    cursor.close()
    cnx.close()
