# -*- coding: utf-8 -*-
"""
Created on Mon Feb 28 11:11:05 2022

@author: Administrator
"""
import pandas as pd
import sqlite3
#import os
path=open("path",encoding='utf-8')
p=path.read()
conn=sqlite3.connect('info.db')
cursor=conn.cursor()
cursor.execute("select * from student")
result=cursor.fetchall()
print(result)
res=pd.DataFrame(result,columns=["id","学号","姓名","分数","生日"])
print(res)
res.to_excel(p,index=False)
