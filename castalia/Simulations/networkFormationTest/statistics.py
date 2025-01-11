f = open("Castalia-Trace.txt", "r")
line = f.readline()

arr = []
while line:
    arr.append(line)
    line = f.readline()

arr = arr[-101:]
for i in range(len(arr)):
    arr[i] = arr[i][arr[i].index("clusterHead:") + 13 : -1]

print(22, arr.count("22"))
print(27, arr.count("27"))
print(72, arr.count("72"))
print(77, arr.count("77"))
