from mnist import MNIST

mndata = MNIST('Data/mnist')

images, labels = mndata.load_training() #load raw data

#now write out to easier to parse format - each image is 784 bytes of binary - compress to 98 bytes + 1 label byte in front
for i in range(0, len(images)):
    fi = open("Data/mnist/parsed_mnist/training/tr_img_" + str(i), "wb")
    fi.write(labels[i].to_bytes(1, 'big'))

    for j in range(0, 98): #collect individual bytes 
        byte = 0 
        for k in range(0, 8):
             byte += images[i][8 *j + k] * (1 << (7 - k)) #set bits individually, starting with most significant bit
        fi.write(byte.to_bytes(2, 'big')[0:1])

    print("\r wrote file tr_img_" + str(i) + "    ")
    fi.close()

#now do the exact same thing for the test set
images, labels = mndata.load_testing()
for i in range(0, len(images)):
    fi = open("Data/mnist/parsed_mnist/test/test_img_" + str(i), "wb")
    fi.write(labels[i].to_bytes(1, 'big'))

    for j in range(0, 98): #collect individual bytes 
        byte = 0 
        for k in range(0, 8):
             byte += images[i][8 *j + k] * (1 << (7 - k)) #set bits individually, starting with most significant bit
        fi.write(byte.to_bytes(2, 'big')[0:1])

    print("\r wrote file test_img_" + str(i) + "    ")
    fi.close()

