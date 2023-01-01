import pandas as pd
import sqlite3
path=open("path",encoding='utf-8')
p=path.read()
conn=sqlite3.connect('info.db')
cursor=conn.cursor()
cursor.execute("create table student (id int(10)  primary key, numb int(10), name varchar(20), mark int(100), birth int(100))")
df=pd.read_excel(p)
print(df)
for i in df.index:
    insert="insert into student (id, numb, name, mark) values ("
    insert+="\""+str(i)+"\","
    for j in df.columns:
        insert+="\""+str(df.at[i,j])+"\","
    if(insert[-1]==','):
        insert=insert[0:-1];
    insert+=")"
    print(insert)
    cursor.execute(insert)
cursor.execute("create table setting (hour int(10), minute int(10), addmark int(10))")
cursor.execute('insert into setting (hour, minute, addmark) values ("6","0","3")')
cursor.execute('insert into setting (hour, minute, addmark) values ("6","10","2")')
cursor.execute('insert into setting (hour, minute, addmark) values ("6","20","1")')
cursor.execute('insert into setting (hour, minute, addmark) values ("6","30","0")')
cursor.execute("create table record ( name varchar(20), addmark int(100), datetimeweek varchar(20), reason varchar(200))")
cursor.close()
conn.commit()
conn.close()
