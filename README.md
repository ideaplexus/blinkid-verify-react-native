# BlinkIDVerify SDK wrapper for React Native

This repository contains example wrapper for BlinkIDVerify native SDK for Android and iOS. For 100% of features and maximum control, consider using native SDK.


### Licensing

- Get information about pricing and licensing and to obtain valid license keys, please contact us at http://help.microblink.com

## React Native Version

BlinkIDVerify React Native was built and tested with [React Native v0.48.3](https://github.com/facebook/react-native/releases/tag/v0.48.3)

## Dependencies
`MicroBlink.framework/MicroBlink` file in the iOS plugin exceeds GitHubs limited file size of 100MB. Libraries `LibPhotoPay.aar` and `LibVerification.aar` in the Android plugin are also relatively large.

To correctly fetch these files, Git LFS is needed. Git LFS can be installed with homebrew:

```shell
brew install git-lfs
git lfs install
```

After installation, don't forget to restart the terminal!


## Installation

First generate an empty project if needed:

```shell
react-native init NameOfYourProject
```

Add the **blinkid-verify-react-native** module to your project:

```shell
cd <path_to_your_project>
npm i --save <path_to_plugin_folder>
```

Link module with your project: 

```shell
react-native link blinkid-verify-react-native
```

Further setup is needed for Android and iOS platforms.

## Android Installation and Settings

Add the flatDir repository which contains Microblink libraries to build.gradle:

```
allprojects {
  repositories {
    // don't forget to add maven and jcenter
    mavenLocal()
    jcenter()
    
    // ... other repositories your project needs
    
    flatDir{
      dirs "\$rootDir/../node_modules/blinkid-verify-react-native/src/android/lib"
    }
  }
}
```

## iOS Installation and Settings

There are a couple steps you need to do manually to run your application on iOS.

1. Make sure that you copy **_Microblink.framework_** from `plugin/src/Frameworks/IDVerifyFramework` to iOS project directory.
2. Now open `<ProjectName>/ios/<ProjectName>.xcodeproj` in Xcode. 
3. Drag **_Microblink.framework_** from finder to **Xcode navigator**, and add it to the **embedded binaries** of your project target under **General**.
4. In Build Phases of your project target add **_Microblink.bundle_** and *IDVerify.bundle* to **Copy Bundle Resources**. 
      * You can find both Microblink and IDVerify bundles in your project navigator in:
          1. `Libraries/BlinkIDVerifyReactNative.xcodeproj/Frameworks/IDVerifyFramework.xcodeproj/Frameworks` 
          2. `Libraries/BlinkIDVerifyReactNative.xcodeproj/Frameworks/IDVerifyFramework.xcodeproj/Products`
5. For every target in your project set your team in **General**.
6. Add `'Privacy - Camera Usage Description'` with the description under **Info**.
7. You should now be able to run your application

## Demo

This repository contains **initReactNativeDemoApp.sh** script that will create React Native project and download all of its dependencies. Script is located inside the "sample" folder. Navigate to "sample" folder and run this command: 

```shell
./initReactNativeDemoApp.sh
```

After running the script Android setup is already done, but iOS setup needs to be done manually please follow **iOS Installation and Settings**.

## Usage

To use the module you call it in your index.android.js or index.ios.js file, like the example below:

```javascript
/**
 * Sample React Native App for BlinkIDVerify
 */

 /**
* Use these recognizer types
* Available: RECOGNIZER_MRTD_COMBINED
* RECOGNIZER_MRTD_COMBINED - scans face image from the document and Machine Readable Zone from Machine Readable Travel Document
*/

/**
 * There are several options you need to pass to scan function to add recognizers and to obtain the image and results:
 * enableBeep : if it is set to true, successful scan will play a sound
 * shouldReturnCroppedImages : if true, cropped document images in the recognition process will be returned
 * shouldReturnFaceImages : if true, cropped face image from the document and selfie face image will be returned
 * combinedRecognizer : combined recognizer that will be used for document scan
 */

/**
 * Scan method returns scan fields in JSON format and image(s) (image is returned as Base64 encoded JPEG)
 * scanningResult.resultImageCroppedFront : cropped front side document image
 * scanningResult.resultImageCroppedBack : cropped back side document image
 * scanningResult.resultImageCroppedIDFace : cropped face image from the document
 * scanningResult.resultImageCroppedSelfieFace : cropped selfie face image
 * scanningResult.recognitionResult : document scanning result in JSON format
 */


import React, { Component } from 'react';
import {BlinkIDVerify, MRTDKeys} from 'blinkid-verify-react-native';
import {
  AppRegistry,
  Platform,
  StyleSheet,
  Text,
  View,
  Image,
  ScrollView,
  Button
} from 'react-native';

// Microblink SDK license key bound to application ID for Android or iOS.
// To obtain valid license keys, please contact us at http://help.microblink.com
const licenseKeyMicroblinkSDK = Platform.select({
      // license key for MicroBlink SDK

      // iOS license key for applicationID: org.reactjs.native.example.BlinkIDVerifyReactNativeDemo
      ios: 'KMILWENV-KHVRKRKH-T65BCGEI-JKIQZLZO-RYTHLYX3-AANVYSLK-TM7CI4UE-YHBC33C6',
      // android license key for applicationID: com.blinkidverifyreactnativeDemo
      android: 'LDABUFAJ-7YSPXWZ4-ODMJDEDN-UT4DBX4V-57M5TWOZ-3HM5TWOZ-3HM5TWOZ-3HM3ABKY'
})

// License key for the liveness recognizer which is used for user liveness recognition and taking the selfie picture
// If the license key is not set, liveness recognition will work, but it will stop after 60 seconds
const licenseKeyLivenessRecognizer = Platform.select({
      // license key for liveness recognizer

      // iOS license key for applicationID: org.reactjs.native.example.BlinkIDVerifyReactNative
      ios: '<ios_license_key>',
      // android license key for applicationID: com.blinkidverifyreactnative
      android: '<android_license_key>'

})

var renderIf = function(condition, content) {
  if (condition) {
      return content;
  } 
  return null;
}

export default class BlinkIDVerifyReactNative extends Component {
  constructor(props) {
    super(props);
    this.state = {showImageCroppedFront: false,
                  resultImageCroppedFront: '',
                  showImageCroppedBack: false,
                  resultImageCroppedBack: '',
                  showImageIDFace: false,
                  resultImageCroppedIDFace: '',
                  showImageSelfieFace: false,
                  resultImageCroppedSelfieFace: '',
                  results: '',
                  licenseKeyErrorMessage: ''};
  }
  async scan() {
    try {
      const scanningResult = await BlinkIDVerify.scan(
        licenseKeyMicroblinkSDK,
        {
          enableBeep: true,
          shouldReturnCroppedImages: true,
          shouldReturnFaceImages: true,
          combinedRecognizer: BlinkIDVerify.RECOGNIZER_MRTD_COMBINED
        },
        licenseKeyLivenessRecognizer
      )
      if (scanningResult) {
        let recognitionResult = scanningResult.recognitionResult;
        let resultFormattedText = "";
        let fieldDelim = ";\n";

        resultFormattedText += "Result type: " + recognitionResult.resultType + fieldDelim;
        if (recognitionResult.resultType == "MRTD result") {
              var fields = recognitionResult.fields
              // MRTDKeys are keys from keys/mrtd_keys.js
              resultFormattedText += /** Personal information */
                                      "Family name: " + fields[MRTDKeys.PrimaryId] + fieldDelim +
                                      "First name: " + fields[MRTDKeys.SecondaryId] + fieldDelim +
                                      "Date of birth: " + fields[MRTDKeys.DateOfBirth] + fieldDelim +
                                      "Sex: " + fields[MRTDKeys.Sex] + fieldDelim +
                                      "Nationality: " + fields[MRTDKeys.Nationality] + fieldDelim +
                                      "Date of Expiry: " + fields[MRTDKeys.DateOfExpiry] + fieldDelim +
                                      "Document Code: " + fields[MRTDKeys.DocumentCode] + fieldDelim +
                                      "Document Number: " + fields[MRTDKeys.DocumentNumber] + fieldDelim +
                                      "Issuer: " + fields[MRTDKeys.Issuer] + fieldDelim +
                                      "Opt1: " + fields[MRTDKeys.Opt1] + fieldDelim +
                                      "Opt2: " + fields[MRTDKeys.Opt2] + fieldDelim;

        }
        resultFormattedText += '\n';
        // image is returned as base64 encoded JPEG, we expect resultImageCorppedFront and resultImageCorppedBack because we have activated obtaining of 
        // cropped images (shouldReturnCroppedImages: true)
        this.setState({showImageCroppedFront: scanningResult.resultImageCroppedFront, resultImageCroppedFront: 'data:image/jpg;base64,' + scanningResult.resultImageCroppedFront,
                       showImageCroppedBack: scanningResult.resultImageCroppedBack, resultImageCroppedBack: 'data:image/jpg;base64,' + scanningResult.resultImageCroppedBack,
                       showImageSelfieFace: scanningResult.resultImageCroppedSelfieFace, resultImageCroppedSelfieFace: 'data:image/jpg;base64,' + scanningResult.resultImageCroppedSelfieFace,
                       showImageIDFace: scanningResult.resultImageCroppedIDFace, resultImageCroppedIDFace: 'data:image/jpg;base64,' + scanningResult.resultImageCroppedIDFace, results: resultFormattedText});}
    } catch(error) {
        this.setState({ showImageCroppedFront: false, resultImageCroppedFront: '', showImageCroppedBack: false, resultImageCroppedBack: '', showImageIDFace: false, resultImageCroppedIDFace: '', showImageSelfieFace: false, resultImageCroppedSelfieFace: '', results: error.message});
    }
    
  }

  render() {
    let displayImageCroppedFront = this.state.resultImageCroppedFront;
    let displayImageCroppedBack = this.state.resultImageCroppedBack;
    let displayImageIDFace = this.state.resultImageCroppedIDFace;
    let displayImageSelfieFace = this.state.resultImageCroppedSelfieFace;
    let displayFields = this.state.results;
    let licenseKeyErrorMessage = this.state.licenseKeyErrorMessage;
    return (
      <View style={styles.container}>
        <Text style={styles.label}>MicroBlink Ltd</Text>
        <View style={styles.buttonContainer}>
          <Button
            onPress={this.scan.bind(this)}
            title="Scan"
            color="#87c540"
          />
        </View>
        <ScrollView
          automaticallyAdjustContentInsets={false}
          scrollEventThrottle={200}y> 
          {renderIf(this.state.showImageCroppedFront,
              <View style={styles.imageContainer}>
              <Image
                resizeMode='contain'
                source={{uri: displayImageCroppedFront, scale: 3}} style={styles.imageResult}/>
              </View>
          )}
          {renderIf(this.state.showImageCroppedBack,
              <View style={styles.imageContainer}>
              <Image
                resizeMode='contain'
                source={{uri: displayImageCroppedBack, scale: 3}} style={styles.imageResult}/>
              </View>
          )}
          {renderIf(this.state.showImageIDFace,
              <View style={styles.imageContainer}>
              <Image
                resizeMode='contain'
                source={{uri: displayImageIDFace, scale: 3}} style={styles.imageResult}/>
              </View>
          )}
          {renderIf(this.state.showImageSelfieFace,
              <View style={styles.imageContainer}>
              <Image
                resizeMode='contain'
                source={{uri: displayImageSelfieFace, scale: 3}} style={styles.imageResult}/>
              </View>
          )}
          <Text style={styles.results}>{displayFields}</Text>
        </ScrollView>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    flexDirection: 'column',
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF'
  },
  label: {
    fontSize: 20,
    textAlign: 'center',
    marginTop: 50
  },
  buttonContainer: {
    margin: 20
  },
  imageContainer: {
    flexDirection: 'row',
    justifyContent: 'center'
  },
  results: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  imageResult: {
    flex: 1,
    flexShrink: 1,
    height: 200,
    alignItems: 'center',
    justifyContent: 'center',
    margin: 10
  },
});

AppRegistry.registerComponent('BlinkIDVerifyReactNative', () => BlinkIDVerifyReactNative);

```
+ Available recognizers are:
    + **RECOGNIZER\_MRTD\_COMBINED**  - scans face image from the document and Machine Readable Zone from Machine Readable Travel Document
	
+ Scan method returns scan fields in JSON format and images (image is returned as Base64 encoded JPEG)
	+ **scanningResult.recognitionResult** : document scanning result in JSON format
	+ **scanningResult.resultImageCroppedFront** : cropped front side document image
	+ **scanningResult.resultImageCroppedBack** : cropped back side document image
 	+ **scanningResult.resultImageCroppedIDFace** : cropped face image from the document
   + **scanningResult.resultImageCroppedSelfieFace** : cropped selfie face image

+ LicenseKeyMicroblinkSDK and LicenseKeyLivenessRecognizer parameters must be provided.

## FAQ

**Can I create a custom UI overlay?**

Yes you can, but you will have to implement it natively for android and ios, you can see native implementation guides [here(Android)](https://github.com/BlinkID/blinkid-android#recognizerView) and [here(ios)](https://github.com/BlinkID/blinkid-ios/wiki/Customizing-Camera-UI#steps-for-providing-custom-camera-overlay-view).

**Could not expand ZIP '.../lib/LibPhotoPay.aar'**

The missing libraries were not downloaded. Git LFS is needed for downloading these files (check **Dependencies** section).
 You probably see the files in your file system, but they are not actually downloaded and are just text files. To download you must install Git LFS and repeat `git pull`.