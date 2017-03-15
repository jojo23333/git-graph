import requests
import time

class UserDoesNotExist(Exception):
    def __init__(self):
        pass

class git(object):
    url = "https://api.github.com/"
    
    def __init__(self,username,token):
        self.header  = {"Authorization":"token "+token}
        self.username = username
        res = requests.get(self.url+"users/"+username,headers=self.header,timeout=30)
        if res.status_code is not 200:
            print(res.headers)
            print(self.header)
            raise UserDoesNotExist
        self.status = res.headers
        self.info = res.json()
        self.following_pages = int(self.info['following']/30) + 1
        self.followers_pages = int(self.info['followers']/30) + 1

    def following(self):
        userset =[]
        page = 1 
        while page <= self.following_pages:
            try:
                print('\rfollowing page %d/%d  '%(page,self.following_pages),end='')
                res = requests.get(self.url+"users/"+self.username+"/following?page="+str(page),\
                        headers=self.header,timeout=30)
                #set timeout is needed,otherwise is easily end up in deadlock
                print("get complete",end='')
                userset += [x['login'] for x in res.json()]
                self.status = res.headers
                assert res.status_code == 200
                page = page + 1
            #if somethig wrong about the connection
            except (requests.exceptions.ConnectionError,requests.exceptions.ConnectTimeout,\
                    requests.exceptions.ReadTimeout) as e:
                print(repr(e),"%s at following page %d"%(self.username,page))
                time.sleep(0.3)
                continue
            except AssertionError:
                print("Reach limit of api!wait for %s s"%self.gap())
                time.sleep(self.gap())
            except:
                print(page,' ',res.json())
                raise Exception

        return userset

    def followers(self):
        userset =[]
        page = 1
        while page <= self.followers_pages:
            try:
                print('\rfollower page %d/%d  '%(page,self.followers_pages),end='')
                res = requests.get(self.url+"users/"+self.username+"/followers?page="+str(page),\
                    headers=self.header,timeout=30)
                userset += [x['login'] for x in res.json()]
                self.status = res.headers
                assert res.status_code == 200
                page = page + 1
            #if somethig wrong about the connection
            except (requests.exceptions.ConnectionError,requests.exceptions.ConnectTimeout,\
                    requests.exceptions.ReadTimeout) as e:
                print(repr(e),"%s at followers page %d"%(self.username,page))
                time.sleep(0.3)
                continue
            #if status_code is not right
            except AssertionError:
                print("Reach limit of api!wait for %s s"%self.gap())
                time.sleep(self.gap())
            except:
                print(page,' ',res.json())
                raise Exception
        return userset

    def gap(self):
        t = float(self.status['X-RateLimit-Reset']) - time.time()
        limit = int(float(self.status['X-RateLimit-Remaining']))
        if limit is not 0:
            return float((t+1)/limit)
        else:
            return t



