from mysql_api import mysql_init,config,data
from grab import git,UserDoesNotExist
from datetime import date
from collections import deque
import logging
import traceback
import time

mysql_init()
sql = data()

#Config logging module
logging.basicConfig(level=logging.INFO,
                format='%(asctime)s %(levelname)s %(message)s',
                datefmt='%a, %d %b %Y %H:%M:%S',
                filename='data.log',
                filemode='w')
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('%(levelname)-8s %(message)s')
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)

userset = set()
errorlog = open('Error.log',"a")
backup = open('backup','w')
#ignoredlist = open('Ignoredlist','w')

def write_info(infolist,following,followers):
    stored_keys = ['id','login','avatar_url','name','email','location','bio','public_repos',
    'public_gists','followers','following','created_at']
    userinfo = {}
    for x in infolist.keys():
        if x in stored_keys:
            userinfo[x] = infolist[x]
    time = userinfo['created_at']
    userinfo['created_at'] = date(int(time[0:4]),int(time[5:7]),int(time[8:10]))
    #insert personal information
    sql.insert('userinfo',**userinfo)

    #insert followers and following's infos 
    for x in following:
        sql.insert('following',userid=userinfo['login'],followingid=x)
    for x in followers:
        sql.insert('follower',userid=userinfo['login'],followerid=x)
    #backup part
    backup.write(userinfo['login']+'\n')
    backup.write('following:'+str(following)+'\n')
    backup.write('followers:'+str(followers)+'\n')


def BFS_grab():
    while not userlist.empty():
        username = userlist.popleft()
        try:
            begin=time.time()
            user = git(username,token)

            if user.followers_pages > 30:
                userlist.append(x)
                logging.info("%s ignored."%username)
                continue
            #priority for not_key_position users.Query for than 10000 followers consume great amount of time

            logging.info("#Collecting info of the user {}.".format(username))
            following = user.following()
            followers = user.followers()

            logging.info("Writing %s's info"%username)
            write_info(user.info,following,followers)
            
            logging.info(str(time.time()-begin)+'s')
        except UserDoesNotExist:
            logging.warning("User {} does not exist or token is invalid!".format(username))
        except KeyboardInterrupt:
            s = open("save","w")
            s.write(str(list(userlist))+'\n'+str(list(keyuserlist))+'\n'+str(userset))
            s.close()
            break
        except Exception:
            errorlog.write(str(time.asctime(time.localtime(time.time())))+":"+'\n')
            traceback.print_exc(file=errorlog)
            traceback.print_exc() 
            #write the traceback into errolog and console
        else:
            logging.info('pending time :%f'%user.gap())
            time.sleep(user.gap())
            #This is used to avoid api rating limit exceeded
            for x in following:
                if x not in userset and sql.recorded(x)!=True:
            #Judge if the user's info is grabbed 
                    userlist.append(x)
                    userset.add(x)
            logging.info("User:{}'s information collected.".format(username))

#Create your own .token file containing your token
t = open(".token","r")
token = t.readline().strip('\n')
t.close()

t = open("save","r")
userlist = deque([x for x in t.readline().strip("[']\n").split("'") x!=", "])
keyuserlist= deque([x for x in t.readline().strip("[']\n").split("'") x!=", "])
userset= set([x for x in t.readline().strip("{'}\n").split("'") x!=", "])

BFS_grab()



