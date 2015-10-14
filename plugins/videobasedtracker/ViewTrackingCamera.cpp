/** @file
    @brief Implementation

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2015 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include "DirectShowToCV.h"
#include "directx_camera_server.h"

// Library/third-party includes
#include <opencv2/highgui/highgui.hpp>

// Standard includes
#include <memory>
#include <iostream>

/// @brief OpenCV's simple highgui module refers to windows by their name, so we
/// make this global for a simpler demo.
static const std::string windowNameAndInstructions(
    "OSVR tracking camera preview | q or esc to quit");

// This string begins the DevicePath provided by Windows for the HDK's camera.
static const auto HDK_CAMERA_PATH_PREFIX = "\\\\?\\usb#vid_0bda&pid_57e8&mi_00";

int main() {
    auto cam = std::unique_ptr<directx_camera_server>{
        new directx_camera_server(HDK_CAMERA_PATH_PREFIX)};
    if (!cam->read_image_to_memory()) {
        std::cerr
            << "Couldn't find, open, or read from the OSVR HDK tracking camera."
            << std::endl;
        return -1;
    }
    auto frame = cv::Mat{};

    cv::namedWindow(windowNameAndInstructions);
    do {
        frame = retrieve(*cam);
        cv::imshow(windowNameAndInstructions, frame);
        char key = static_cast<char>(cv::waitKey(1)); // wait 1 ms for a key
        if ('q' == key || 'Q' == key || 27 /*esc*/ == key) {
            break;
        }
    } while (cam->read_image_to_memory());
    return 0;
}
