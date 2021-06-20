import mariadb
import sys
import argparse

from datetime import date, datetime

try:
    myConnection=mariadb.connect(
        user='root',
        password='root',
        host='65.0.110.114',
        port=59002,
    )

except mariadb.Error as e:
    print(f'[Error] {e}')
    sys.exit(1)

myCursor=myConnection.cursor()

def createDatabase(myCursor):
    myCursor.execute("CREATE DATABASE IF NOT EXISTS `thermique`")


def createRecordsTable(myCursor):
    createDatabase(myCursor)
    
    myCursor.execute('USE `thermique`')
    #this ^ line may be removed if we specify a "database" parameter in mariadb.connect()
    
    myCursor.execute(
        "CREATE TABLE IF NOT EXISTS `records` (`id` bigint(20) UNSIGNED NOT NULL,`created_at` timestamp NULL DEFAULT NULL,`updated_at` timestamp NULL DEFAULT NULL,`person_id` int(11) NOT NULL,`temperature` double(8,2) NOT NULL,`timestamp` datetime NOT NULL,`gate` int(11) NOT NULL) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci"
        #person_id, time, date & gate should altogether make a composite key, but that doesn't have any use at the moment        
    )
    
#    myCursor.execute(
#        "ALTER TABLE `records` ADD PRIMARY KEY (`id`)"
#        #Adding primary key constraint
#    )
#    
#    myCursor.execute(
#        "ALTER TABLE `records` MODIFY `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10"
#        #auto-increment or something idk laravel theke dise
#    )


def addRecord(myCursor, person_id, temperature, timestamp, gate):
    createRecordsTable(myCursor)
    print('person_id:{0}\ntemperature:{1}\ntimestamp:{2}\ngate:{3}'.format(person_id, temperature, timestamp, gate))
    myCursor.execute(
        #f"SELECT person_id FROM `records` WHERE `person_id`={person_id} AND `date`='{date}' AND TIMEDIFF('{time}',`time`) <= '00:00:30'"
        f"SELECT person_id FROM `records` WHERE `person_id`={person_id} AND TIMESTAMPDIFF(SECOND,`timestamp`,'{timestamp}') <= '30' AND TIMESTAMPDIFF(SECOND,`timestamp`,'{timestamp}') > '0'"
        #this ^ line either returns a single row, otherwise empty row
    )

    redundant=myCursor.fetchall()
    #if the SELECT query returns an empty row, variable "redundant" is set to [], or None, which is logically equivalent to False.
    #otherwise "redundant" will store [(person_id,)]
    #yep, it's a tuple inside an array ^
    
    
    if(not redundant):
        myCursor.execute(
            #f"INSERT INTO `records` VALUES ('{person_id}', '{temperature}', '{time}', '{date}', '{gate}')"
            f"INSERT INTO `records` (`created_at`, `updated_at`, `person_id`, `temperature`, `timestamp`, `gate`) VALUES (CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'{person_id}', '{temperature}', '{timestamp}', '{gate}')"
        )
    else:
        myCursor.execute(
            #f"UPDATE `records` SET `temperature`='{temperature}', `time`='{time}' WHERE `person_id`={person_id} AND `date`='{date}' AND TIMEDIFF('{time}',`time`) <= '00:00:30'"
            f"UPDATE `records` SET `updated_at`=CURRENT_TIMESTAMP, `temperature`='{temperature}', `timestamp`='{timestamp}' WHERE `person_id`={person_id} AND TIMESTAMPDIFF(SECOND,`timestamp`,'{timestamp}') <= '30' AND TIMESTAMPDIFF(SECOND,`timestamp`,'{timestamp}') > '30'"
        ) 

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    
    parser.add_argument("-id")
    parser.add_argument("-timestamp")
    parser.add_argument("-temperature")
    
    args=parser.parse_args()

    person_id = args.id
    temperature = args.temperature
    #today_date, current_time = datetime.utcfromtimestamp(int(args.timestamp)).strftime("%Y-%m-%d %H:%M:%S").split(" ")
    timestamp = datetime.utcfromtimestamp(int(args.timestamp)).strftime("%Y-%m-%d %H:%M:%S")
    gate = 1

    addRecord(myCursor, person_id, temperature, timestamp, gate)


    # myCursor.execute(f"SELECT * FROM `records`")
    # for(person_id, temperature, time, date, gate) in myCursor:
    #     print(f'{person_id}\t{temperature}\t{time}\t{date}')
    myConnection.commit()
    #this ^ line is included because, whenever connection is established without a "database" parameter, autocommit is set to False

    myConnection.close()