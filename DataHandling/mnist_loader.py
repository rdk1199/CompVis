from mnist import MNIST

mndata = MNIST('Data/mnist')

width = 28
height = 28

images, labels = mndata.load_training() #load raw data

#now write out to easier to parse format - each image is 784 bytes of binary - compress to 98 bytes + 1 label byte in front
for i in range(0, len(images)):
    fi = open("Data/mnist/parsed_mnist/training/tr_img_" + str(i), "wb")
    fi.write(labels[i].to_bytes(1, 'little'))  

    fi.write(width.to_bytes(4, 'little')) #write width, then height 
    fi.write(height.to_bytes(4, 'little'))

    for j in range(0, 28 * 28): #collect individual bytes 
        fi.write(images[i][j].to_bytes(1, 'little'))

    print("\r wrote file tr_img_" + str(i) + "    ")
    fi.close()

print(mndata.display(images[0]))

#now do the exact same thing for the test set
images, labels = mndata.load_testing()
for i in range(0, len(images)):
    fi = open("Data/mnist/parsed_mnist/test/test_img_" + str(i), "wb")
    fi.write(labels[i].to_bytes(1, 'little'))

    fi.write(width.to_bytes(4, 'little')) #write width, then height 
    fi.write(height.to_bytes(4, 'little'))

    for j in range(0, 28*28): #collect individual bytes    
        fi.write(images[i][j].to_bytes(1, 'little'))

    print("\r wrote file test_img_" + str(i) + "    ")
    fi.close()

