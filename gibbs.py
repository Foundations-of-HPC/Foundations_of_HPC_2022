from __future__ import division
import cv2
import numpy as np
import argparse
import sys
import matplotlib.pyplot as plt
from pylab import *
from PIL import Image
from pylab import rcParams

#from pylab import rcParams
#rcParams['figure.figsize'] = 6, 6

def main():
    rcParams['figure.figsize'] = 6, 6
    parser = argparse.ArgumentParser()
    parser.add_argument("--image", type = str, default = "MArylin.JPG", help = "What is the name of the image?")
    parser.add_argument("--iter", type = int, default = 6000000, help = "Number of iterations you want for ICM")
    parser.add_argument("--sigma2", type = float, default = 1, help = "Value of sigma_squared")
    parser.add_argument("--beta", type = float, default = 0.8, help = "Value of beta")

    args = parser.parse_args()
    #sys.stdout.write(str(ICM(args)))

    # Saving command line parameters into variables
    image = args.image
    T = args.iter
    sigma_squared = args.sigma2
    beta = args.beta

    imgX = Image.open(image)  # open colour image
    imgX = imgX.convert('L')

    img = np.asarray(imgX)
    img = 2*(img > 128).astype(int)-1

    #create a noisy binary image
    noise = np.random.random(img.shape)
    noise = np.where(noise<0.15,-1,1)
    Y = np.array(img*noise) #observed noisy binary image.

    noisy_im = Image.fromarray((np.where(Y == -1, 0, 1)*255).astype(np.uint8), mode = "L")
    noisy_im.save("noisyimg.jpg")

    height,width = Y.shape
    N = height*width
    
    errorG= np.zeros(T)

    X = Y.copy() # Initialize Z from X
    for t in range(T):

        if(t%100000==0):
            np.save('arrayG.npy',errorG)
            np.save('ImageG.npy', X)
            np.save('iteration',t)

        i,j = np.random.choice(height), np.random.choice(width) # randomly choose a coordinate (i,j)


        # Calculate acceptance propability
        # We don't take exponential. Instead, we take the log of random number. This is more stable.
        energy =  beta*(np.sum(X[max(i-1,0):i+2,max(j-1,0):j+2]) - X[i,j])
        term1=np.exp(energy -(0.5/sigma_squared)*(Y[i,j]-1)**2)
        term2=np.exp(-energy - (0.5/sigma_squared)*(Y[i,j]+1)**2)
        Prob_Xij_one_given_neighbors = float(term1)/(term1+term2)
        Prob_Xij_minus_one_given_neighbors = 1.0 - Prob_Xij_one_given_neighbors #float(term2)/(term1+term2)

        # We also don't deal with minimum function for acceptance probability
        #   because we don't need to cast it to 1 if it is higher.

        # Flip the pixel if accepted

        X[i,j]=-1 if np.random.rand()<=Prob_Xij_minus_one_given_neighbors else 1

        errorG[t]=np.sum(img != X)

    result = Image.fromarray((np.where(X == -1, 0, 1)*255).astype(np.uint8), mode = "L")
    result.save("result.jpg")
    np.save('arrayG.npy',errorG)
    np.save('ImageG.npy', X)
    x = np.linspace(0, T, T)
    # Plot the array against x
    plt.plot(x, (errorG*100/(width*height)))
    plt.xlabel('Iteration #')
    plt.ylabel('Error %')
    plt.title('Evolution of Metropolis-Hastings %error w.r.t the number of iterations')
    plt.grid(True)
    plt.savefig('error-plot.png')
    
    #plot_figures(imgX, noisy_im, result)

def plot_figures(image1, image2, image3):

    width, height = image2.size
    new_width = width * 3
    new_size = (new_width, height)

    image1 = image1.resize(new_size)
    image2 = image2.resize(new_size)
    image3 = image3.resize(new_size)

    # Create a new blank image with the required width
    merged_image = Image.new("RGB", (new_width, height))

    # Paste the three images side by side
    merged_image.paste(image1, (0, 0))
    merged_image.paste(image2, (width, 0))
    merged_image.paste(image3, (width * 2, 0))

    # Save the merged image
    merged_image.save("merged_image.jpg")

if __name__ == '__main__':
    main()