token = '705ebfc75907cd611458ade90dd30cbf391c95a3'

import requests
import time

class UserDoesNotExist(Exception):
    def __init__():
        pass

class git(object):
    header = {"Authorization":"token 705ebfc75907cd611458ade90dd30cbf391c95a3"}
    url = "https://api.github.com/"
    
    def __init__(self,username):
        self.username = username
        res = requests.get(self.url+"users/"+username)
        if res.status_code is not 200:
            raise UserDoesNotExist
        self.status = res.header
        self.info = res.json()
        self.following_pages = int(self.info['following']/30) + 1
        self.followers_pages = int(self.info['followers']/30) + 1

    def following(self):
        userset =[]
        for page in range(1,self.following_pages):
            res = requests.get(self.url+"users/"+self.username+"/following?page="+str(page))
            userset += [x['login'] for x in res.json()]
        self.status = res.header
        return userset

    def followers(self):
        res = requests.get(self.url+"users/"+self.username+"/followers")
        for page in range(1,self.followers_pages):
            res = request.get(self.url+"users/"+self.username+"/followers?page="+str(page))
            userset += [x['login'] for x in res.json()]
        self.status = res.header
        return res

    def gap(self):
        time = time.time() - float(self.header['X-RateLimit-Reset'])
        limit = int(float(self.header['X-RateLimit-Remaining'])

        return (time+1)/limit



