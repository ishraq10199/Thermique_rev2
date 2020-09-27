import mariadb
import sys
import argparse

from datetime import date, datetime

try:
    myConnection=mariadb.connect(
        user='root',
        password='0',
        host='192.168.1.71',
        port=3306,
    )#please change this as per required

except mariadb.Error as e:
    print(f'[Error] {e}')
    sys.exit(1)

myCursor=myConnection.cursor()

def createDatabase(myCursor):
    myCursor.execute("CREATE DATABASE IF NOT EXISTS `thermique_db`")


def createRecordsTable(myCursor):
    createDatabase(myCursor)
    
    myCursor.execute('USE `thermique_db`')
    #this ^ line may be removed if we specify a "database" parameter in mariadb.connect()
    
    myCursor.execute(
        "CREATE TABLE IF NOT EXISTS `records`(`person_id` INT(11), `temperature` DOUBLE, `time` TIME, `date` DATE, `gate` TINYINT)"
        #person_id, time, date & gate should altogether make a composite key, but that doesn't have any use at the moment
    )


def addRecord(myCursor, person_id, temperature, time, date, gate):
    createRecordsTable(myCursor)
    print('person_id:{0}\ntemperature:{1}\ntime:{2}\ndate:{3}\ngate{4}'.format(person_id, temperature, time, date, gate))
    myCursor.execute(
        f"SELECT person_id FROM `records` WHERE `person_id`={person_id} AND `date`='{date}' AND TIMEDIFF('{time}',`time`) <= '00:00:30'"
        #this ^ line either returns a single row, otherwise empty row
    )

    redundant=myCursor.fetchall()
    #if the SELECT query returns an empty row, variable "redundant" is set to [], or None, which is logically equivalent to False.
    #otherwise "redundant" will store [(person_id,)]
    #yep, it's a tuple inside an array ^
    
    
    if(not redundant):
        myCursor.execute(
            f"INSERT INTO `records` VALUES ('{person_id}', '{temperature}', '{time}', '{date}', '{gate}')"
        )
    else:
        myCursor.execute(
            f"UPDATE `records` SET `temperature`='{temperature}', `time`='{time}' WHERE `person_id`={person_id} AND `date`='{date}' AND TIMEDIFF('{time}',`time`) <= '00:00:30'"
        ) 

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    
    parser.add_argument("-id")
    parser.add_argument("-timestamp")
    parser.add_argument("-temperature")
    
    args=parser.parse_args()

    person_id = args.id
    temperature = args.temperature
    current_time, today_date = datetime.utcfromtimestamp(int(args.timestamp)).strftime("%H:%M:%S %Y-%m-%d").split(" ")
    gate = 1

    addRecord(myCursor, person_id, temperature, current_time, today_date, gate)


    # myCursor.execute(f"SELECT * FROM `records`")
    # for(person_id, temperature, time, date, gate) in myCursor:
    #     print(f'{person_id}\t{temperature}\t{time}\t{date}')
    myConnection.commit()
    #this ^ line is included because, whenever connection is established without a "database" parameter, autocommit is set to False

    myConnection.close()