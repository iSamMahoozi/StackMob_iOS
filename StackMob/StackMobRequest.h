// Copyright 2011 StackMob, Inc
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#import "StackMobSession.h"
#import "StackMobConfiguration.h"
#import "StackMobQuery.h"
#import "External/RestKit/Vendor/JSONKit/JSONKit.h"
#import <RestKit/RestKit.h>

@class StackMob;
typedef void (^StackMobCallback)(BOOL success, id result);

typedef enum {
	GET,
	POST,
	PUT,
	DELETE
} SMHttpVerb;

@interface StackMobRequest : NSObject <RKRequestDelegate>
{
	NSURLConnection*		mConnection;
	SEL						mSelector;
    BOOL                    mIsSecure;
    NSString*				mMethod;
	NSMutableDictionary*	mArguments;
    NSMutableDictionary*    mHeaders;
	NSMutableData*			mConnectionData;
	NSDictionary*			mResult;
    NSError*                mConnectionError;
	BOOL					_requestFinished;
	NSHTTPURLResponse*		mHttpResponse;
    RKRequest*              mBackingRequest;
    StackMobCallback        mCallback;
	
	@protected
    BOOL userBased;
	StackMobSession *session;
}

@property(readwrite, copy) NSString* method;
@property(readwrite, assign, getter=getHTTPMethod, setter=setHTTPMethod:) SMHttpVerb httpMethod;
@property(readwrite) BOOL isSecure;
@property(readwrite, retain) NSDictionary* result;
@property(readwrite, retain) NSError* connectionError;
@property(readonly) BOOL finished;
@property(readonly) NSHTTPURLResponse* httpResponse;
@property(readonly, getter=getStatusCode) NSInteger statusCode;
@property(readonly, getter=getBaseURL) NSString* baseURL;
@property(readonly, getter=getURL) NSURL* url;
@property(readonly, getter=getResourcePath) NSString* resourcePath;
@property(nonatomic) BOOL userBased;
@property(readwrite, retain) RKRequest* backingRequest;
@property(readwrite, retain) StackMobCallback callback;

/* 
 * Standard CRUD requests
 */
+ (id)request;
+ (id)requestForMethod:(NSString*)method;
+ (id)requestForMethod:(NSString*)method withHttpVerb:(SMHttpVerb) httpVerb;
+ (id)requestForMethod:(NSString*)method withArguments:(NSDictionary*)arguments withHttpVerb:(SMHttpVerb) httpVerb;
+ (id)requestForMethod:(NSString*)method withQuery:(StackMobQuery *)query withHttpVerb:(SMHttpVerb) httpVerb;

/* 
 * User based requests 
 * Use these to execute a method on a user object
 */
+ (id)userRequest;
+ (id)userRequestForMethod:(NSString *)method withHttpVerb:(SMHttpVerb)httpVerb;
+ (id)userRequestForMethod:(NSString*)method withArguments:(NSDictionary*)arguments withHttpVerb:(SMHttpVerb)httpVerb;
+ (id)userRequestForMethod:(NSString *)method withQuery:(StackMobQuery *)query withHttpVerb:(SMHttpVerb)httpVerb;

/*
 * Create a request for an iOS PUSH notification
 * @param arguments a dictionary of arguments including :alert, :badge and :sound
 */
+ (id)pushRequestWithArguments:(NSDictionary*)arguments withHttpVerb:(SMHttpVerb) httpVerb;

/**
 * Convert a NSDictionary to JSON
 * @param dict the dictionary to convert to JSON
 */
+ (NSData *)JsonifyNSDictionary:(NSMutableDictionary *)dict withErrorOutput:(NSError **)error;

/*
 * Set parameters for requests
 */
- (void)setArguments:(NSDictionary*)arguments;
/*
 * Set headers for requests, overwrites all headers set for the request
 * @param headers, the headers to set
 */
- (void)setHeaders:(NSDictionary *)headers;

/*
 * Send a configured request and wait for callback
 */
- (void)sendRequest;

/*
 * Send a configured request and wait for callback
 */
- (StackMobRequest*)sendRequestWithCallback:(StackMobCallback)callback;

/*
 * Cancel and ignore a request in progress
 */
- (void)cancel;

// return the post body as NSData
- (NSData *)postBody;


@end

@protocol SMRequestDelegate <NSObject>

@optional
- (void)requestCompleted:(StackMobRequest *)request;

@end


