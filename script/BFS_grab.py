from mysql_api import mysql_init,config,data,InsertionError
from grab import git,UserDoesNotExist
import time
import Queue

mysql_init()

userlist = Queue.Queue()
userset = set()
sql = data()

def write_info(userinfo,following,followers):
    stored_keys = ['id','login','avatar','name','email','location','bio','public_repos',
    'public_gists','followers','following','created_at']
    for x in userinfo.keys():
        if x not in stored_keys:
            userinfo.pop(x)
    time = stored_keys['created_at']
    stored_keys['created_at'] = date(int(time[0:3]),int(time[5:6]),int(time[8:9]))

    sql.insert('userinfo',userinfo)

    for x in following:
        sql.insert('following',userid=userinfo['login'],followingid=x)
    for x in followers:
        sql.insert('follower',userid=userinfo['login'],followerid=x)


def BFS_grab():
    while not userlist.empty():
        username = userlist.get()
        try:
            user = git(username)
            print("Collecting info of the user %s".format(username))
            following = user.following()
            followers = user.followers()
            write_info(user.info,following,followers)
        except UserDoesNotExist:
            print("User %s does not exist!".format(username))
        else except InsertionError:
            print("Some thing wrong with the writing process.")
        else
            time.sleep(user.gap())
            for x in following or followers:
                if x not in userset or sql.recorded(x):
                    userlist.put(x)
                    userset.add(x)
            print("User:%s's information collected.".format(username))





