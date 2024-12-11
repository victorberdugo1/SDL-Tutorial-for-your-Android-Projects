# SDL Tutorial for your Android Projects

This repository contains the source code and instructions from the SDL Tutorial for your Android Projects, as demonstrated in the [YouTube video](https://www.youtube.com/watch?v=BSBISI0sCqo).

## Introduction

This project demonstrates how to integrate the Simple DirectMedia Layer (SDL) library into your Android projects. SDL is a cross-platform development library designed to provide low-level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [Credits](#credits)
  
## Features

- Setting up SDL in an Android project
- Basic example of SDL usage
- Cross-platform compatibility

## Requirements

- Android Studio
- Android SDK
- SDL2 library

## System Requirements

The version of Android Studio used in this project is ```2024.1.2```.

Since this project uses an older version of SDL the project requires java version 8. Download it from [OpenJDK](https://www.openlogic.com/openjdk-downloads?field_java_parent_version_target_id=416&field_operating_system_target_id=436&field_architecture_target_id=391&field_java_package_target_id=All) and install it. 

After installing it, change the JDK which Android Studio is using by going **File > Settings > Build, Execution, Deployment > Build Tools > Gradle** and change the Gradle SDK to the local directory of the JDK like ```C:\Program Files\OpenLogic\jdk-version```.

You need to install the desired Android SDK for the device you want, in this example we are using API level 17, so to install one go to **File > Settings > Languages & Frameworks > C++ > Android SDK** and choose your desired device, in our case since we are using API level 17 we select the option ```Android 4.2 ("Jelly Bean")```

**Note:** This project only was tested in versions of ```API level 17``` and ```30```.

Lastly what u will need to do is to change the gradle version of the gradle-wrapper.properties to use the version 4.8.1, like this: 
```
distributionUrl=https\://services.gradle.org/distributions/gradle-4.8.1-bin.zip
```

## Installation

Follow these steps to set up the project:

1. **Clone the repository:**

    ```sh
    git clone https://github.com/yourusername/SDL-Android-Tutorial.git
    cd SDL-Android-Tutorial
    ```

2. **Open the project in Android Studio:**

    Launch Android Studio and open the project from the directory where you cloned the repository.

3. **Setup SDL:**

    Download the SDL2 library from the [SDL website](https://www.libsdl.org/download-2.0.php) and follow the instructions to include it in your project. Refer to the YouTube tutorial for detailed steps.

## Usage

To run the project:

1. Connect your Android device or start an Android emulator.
2. Click on the "Run" button in Android Studio or use the following command:

    ```sh
    ./gradlew installDebug
    ```

This will compile and install the app on your connected device or emulator.

## Contributing

Contributions are welcome! If you have any suggestions, bug reports, or feature requests, feel free to open an issue or submit a pull request.

1. Fork the repository
2. Create a new branch (`git checkout -b feature/YourFeature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to the branch (`git push origin feature/YourFeature`)
5. Open a pull request

 ## Credits

- Tutorial by [Victor Berdugo](https://www.youtube.com/@PrograMaths220)
- SDL Library by [SDL Development Team](https://www.libsdl.org/)
- Base on video [YouTube video](https://www.youtube.com/watch?v=BSBISI0sCqo)
- Check out my games on the [Google Play Store](https://play.google.com/store/apps/developer?id=Victor+Berdugo&pli=1)

