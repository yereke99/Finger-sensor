create_data_table = """
                     CREATE TABLE data(
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        id_user INTEGER,
                        type  VARCHAR(255),
                        date_time VARCHAR(255),
                        check_ BOOLEAN
                        )
                     """


insert_data_table = """
                     INSERT INTO data(
                        id_user,
                        type,
                        date_time,
                        check_
                        )VALUES(?,?,?,?)
                     """

create_new_finger_table =  """
                           CREATE TABLE new(
                              id INTEGER PRIMARY KEY AUTOINCREMENT,
                              id_user INTEGER,
                              type  VARCHAR(255),
                              date_time VARCHAR(255),
                              check_ BOOLEAN
                           )
                           """                      

insert_new_finger =  """
                     INSERT INTO new(
                        id_user,
                        type,
                        date_time,
                        check_
                        )VALUES(?,?,?,?)
                     """                     
