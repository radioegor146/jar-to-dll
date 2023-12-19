# Manual for dummies

This manual is for people who don't want to waste their time trying to install all stuff that is required

## Steps

### Register account on GitHub

Your account will be required to build all stuff that you'll need

### Create private repository

Create repository that will be required to run GitHub Actions on it

![create-repo-1](images/create-repo-1.png)

Specify the name of repository and mark it private, if you don't want to make your .jar and .dll public

![create-repo-2](images/create-repo-2.png)

### Import code from this repository

Click `Import` to import

![import-1](images/import-1.png)

Specify this repository URL, e.g. `https://github.com/radioegor146/jar-to-dll`

![import-2](images/import-2.png)

Wait for import to complete 

![import-3](images/import-3.png)

After completion go to your repository

![import-4](images/import-4.png)

### Allow GitHub Actions

Navigate to `Settings` tab

![allow-gh-actions-1](images/allow-gh-actions-1.png)

Open `Actions` menu

![allow-gh-actions-2](images/allow-gh-actions-2.png)

Click on `General`

![allow-gh-actions-3](images/allow-gh-actions-3.png)

Allow all actions to be run

![allow-gh-actions-4](images/allow-gh-actions-4.png)

### Enable GitHub Actions

Navigate to `Actions` tab

![enable-gh-actions-1](images/enable-gh-actions-1.png)

Click `Enable Actions on this repository` to enable actions

![enable-gh-actions-2](images/enable-gh-actions-2.png)

### Upload your .jar file and build .dll

Navigate to `Code` tab

![upload-1](images/upload-1.png)

Open `Add file` menu

![upload-2](images/upload-2.png)

Click `Upload files` to upload your .jar file

![upload-3](images/upload-3.png)

Rename your .jar file on your computer to `input.jar` (IMPORTANT!) and drag-and-drop it here

![upload-4](images/upload-4.png)

Ensure that it is uploaded and click `Commit changes`

![upload-5](images/upload-5.png)

How that the file is uploaded, navigate to `Actions` tab

![upload-6](images/upload-6.png)

Here, click on the running workflow

![upload-7](images/upload-7.png)

Wait for action to complete

![upload-8](images/upload-8.png)

Refresh page and verify that there are no warnings. Now you can download you .dll file and use it as you want!

![upload-9](images/upload-9.png)