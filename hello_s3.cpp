// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0
/**
 * Before running this C++ code example, set up your development environment, including your credentials.
 *
 * For more information, see the following documentation topic:
 *
 * https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/getting-started.html
 *
 **/

// snippet-start:[cpp.example_code.s3.hello_s3]
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <iostream>
#include <aws/core/auth/AWSCredentialsProviderChain.h>

#include <aws/kinesisvideo/KinesisVideoClient.h>
#include <aws/kinesisvideo/model/GetDataEndpointRequest.h>

#include <aws/kinesis-video-archived-media/KinesisVideoArchivedMediaClient.h>
#include <aws/kinesis-video-archived-media/model/GetHLSStreamingSessionURLRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::KinesisVideo;
using namespace Aws::KinesisVideo::Model;
using namespace Aws::KinesisVideoArchivedMedia;
using namespace Aws::KinesisVideoArchivedMedia::Model;


/*
 *  A "Hello S3" starter application which initializes an Amazon Simple Storage Service (Amazon S3) client
 *  and lists the Amazon S3 buckets in the selected region.
 *
 *  main function
 *
 *  Usage: 'hello_s3'
 *
 */

int main(int argc, char **argv) {
    Aws::SDKOptions options;
    // Optionally change the log level for debugging.
//   options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;
    Aws::InitAPI(options); // Should only be called once.
    int result = 0;
    {
        Aws::Client::ClientConfiguration clientConfig;
        // Optional: Set to the AWS Region (overrides config file).
         //clientConfig.region = "ap-southeast-2";
         clientConfig.region = Aws::Region::AP_SOUTHEAST_2;
               
        // You don't normally have to test that you are authenticated. But the S3 service permits anonymous requests, thus the s3Client will return "success" and 0 buckets even if you are unauthenticated, which can be confusing to a new user. 
        auto provider = Aws::MakeShared<DefaultAWSCredentialsProviderChain>("alloc-tag");
        auto creds = provider->GetAWSCredentials();
        if (creds.IsEmpty()) {
            std::cerr << "Failed authentication" << std::endl;
        }

        Aws::S3::S3Client s3Client(clientConfig);
        auto outcome = s3Client.ListBuckets();

        if (!outcome.IsSuccess()) {
            std::cerr << "Failed with error: " << outcome.GetError() << std::endl;
            result = 1;
        } else {
            std::cout << "Found " << outcome.GetResult().GetBuckets().size()
                      << " buckets\n";
            for (auto &bucket: outcome.GetResult().GetBuckets()) {
                std::cout << bucket.GetName() << std::endl;
            }
        }

        auto kinesisVideoClient = KinesisVideoClient(clientConfig);

        auto request = GetDataEndpointRequest();
        request.SetStreamName("stream1");
        request.SetAPIName(APIName::GET_HLS_STREAMING_SESSION_URL);

        auto r = kinesisVideoClient.GetDataEndpoint(request);
        auto result = r.GetResult();
        auto endpoint = result.GetDataEndpoint();

        std::cout << "endpoint: " << endpoint << std::endl;

        auto sr = GetHLSStreamingSessionURLRequest();
        sr.SetStreamName("stream1");
        sr.SetPlaybackMode(HLSPlaybackMode::LIVE);
        

        auto arc = KinesisVideoArchivedMediaClient(clientConfig);
        arc.OverrideEndpoint(endpoint);

        auto sresult = arc.GetHLSStreamingSessionURL(sr);
        auto r2 = sresult.GetResult();
        
        auto url = r2.GetHLSStreamingSessionURL();
        std::cout << "url: " << url << std::endl;
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
// snippet-end:[cpp.example_code.s3.hello_s3]
