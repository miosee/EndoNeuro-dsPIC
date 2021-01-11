import csv



file_name = "Rpi1-"
count = 1602666122
extension = "-0.csv"
check_var = 0 ## Specify counter number of first file
all_true= True
while(all_true == True):
    all_name = str(file_name) + str(count) + str(extension)
    line_count = 0
    try:
        with open(all_name) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                line_count = line_count + 1
                for i in range(0,4):
                    if check_var != int(row[i]):
                        print("File :" + str(all_name) + "Line : " + str(line_count) + "Number" + str(row[i]) ) # If bad sample found
                    if check_var == int(row[i]) and check_var != 255:
                        check_var = check_var + 1
                    if check_var == int(row[i]) and check_var == 255:
                        check_var = 0

    except IOError: ## Counter of file number
        print("File not found")
        break
    
    count = count +1

