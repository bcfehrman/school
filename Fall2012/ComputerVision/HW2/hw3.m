%%Grabs an image, converts to gray scale double representation, allows you to re-generate image from
%%less eigenvector/eigenvalue pairs.

desiredRank = 100;

%%Get images
I = imread( 'images.jpg');

%%Convert to gray
Ig = rgb2gray(I);

%%Convert from 8bit to double
Igd = im2double(Ig);

%%Output image rank
rank(Igd)

%%Perform SVD
[U, S, V] = svd(Igd);

%Get the lower rank representation of the image
ILess= U(:,1:desiredRank) * S(1:desiredRank,1:desiredRank) * V(:,1:desiredRank)';

%Plot the original image and the lower rank representation of the iamge
figure
subplot(1, 2, 1)
imshow(Ig)
subplot(1,2,2)
imshow(ILess)