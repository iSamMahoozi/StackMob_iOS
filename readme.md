# Getting Started
1. Clone the repository from GitHub
`git clone git://github.com/stackmob/StackMob_iOS.git`
2. Open the StackMobiOS project in XCode
3.  Build the target "Build Framework" (Note: if not building for iOS 5.0, first edit line 8 and 10 of script/build)
4.  Copy $\{StackMobiOSHome\}/build/Framework/StackMob.framework to your project as a framework
5. Add the following to Other Linker Flags in the build configuration of your project: -ObjC -all_load
6.  Add the following Frameworks to your project:

    - CFNetwork.framework
    - CoreLocation.framework
    - SystemConfirmation.framework
    - YAJLiOS.framework - This is provided as part of our GitHub project. You will find it in the external folder

7. Copy and configure a StackMob.plist in your application's main Bundle

    - copy Demo/DemoApp/DemoApp/StackMob.plist into your Xcode project
    - enter your app and account info

8. You can now make requests to your servers on StackMob using the following pattern

```objective-c
[[StackMob stackmob] post:@"account" withArguments:[loginObject registerUserParams] andCallback:^(BOOL success, id result){
    if(success){
        NSDictionary *info = (NSDictionary *)result;
        loginObject.userName = [info objectForKey:@"username"];
        loginObject.remoteID = [info objectForKey:@"user_id"];
        mode = MRUserModeSetCurrentUser;
        [self nextStep];
    }
    else{
        MRLog(@"failed to create user %@", result);
        [self setLoading:NO];
        [self flashMessage:NSLocalizedString(@"Unable to create user", "user creation error message")];
        [self.delegate newUserController:self failedWithError:result];
    }
}];
```

9. You can register a user with a facebookToken and userName

```objective-c
[[StackMob stackmob] registerWithFacebookToken:token username:"johnny" andCallback:^(BOOL success, id result){
    if(success){
      // Cast result to a NSDictionary* and do something with the UI
      // Alert your delegates
    }
    else{
      // Cast result to an NSError* and alert your delegates
    }
}];
```

10. You can register an Apple Push Notification service device token like this

```objective-c
    - (void)registerForPush
    {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes: 
         (UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound)];
    }
    - (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken 
    {
        NSString *token = [[deviceToken description] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
        token = [[token componentsSeparatedByString:@" "] componentsJoinedByString:@""];
        // Persist your user's accessToken here if you need
        [[StackMob stackmob] registerUserForPushwithArguments:[[User currentUser] deviceTokenParams] andCallback:^(BOOL success, id result){
            if(success){
                // Registered User and alert your delegates
            }
            else{
                // Unable to register device for PUSH notifications 
                // Fiailed.  Alert your delgates
            }
        }];
    }
```
