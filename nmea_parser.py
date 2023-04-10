raw_data = "calibration.txt"
processed_data = raw_data.replace(".txt", "_processed.csv")


with open(raw_data, "r") as raw, open(processed_data, "w") as processed:
    for line in raw:
        if line.startswith("$GNGGA"):
            GNGGA = line.split(",")[0:9]
            sign = ""
            if GNGGA[3] == "S":
                sign = "-"
            latitude = sign + GNGGA[2][:2] + "." + GNGGA[2][2:4] + GNGGA[2][5:]
            sign = ""
            if GNGGA[5] == "W":
                sign = "-"
            longitude = sign + GNGGA[4][1:3] + "." + GNGGA[4][3:5] + GNGGA[4][6:]
            del GNGGA[0]
            del GNGGA[2]
            del GNGGA[3]
            GNGGA[4], GNGGA[5] = GNGGA[5], GNGGA[4]
            GNGGA[1], GNGGA[2] = latitude, longitude
            print(GNGGA)
            processed.write(",".join(GNGGA) + "\n")