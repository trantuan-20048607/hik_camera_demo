#ifndef _HIK_CAMERA_H_
#define _HIK_CAMERA_H_

#include <string>

#include <opencv2/core.hpp>
#include "MvCameraControl.h"

#include "easylogging++/easylogging++.h"
#include "buffer.h"

class HikCamera {
public:
    HikCamera() :
            device_(nullptr),
            stream_running_(false),
            stop_daemon_thread_flag_(false),
            daemon_thread_id_(0) {}

    HikCamera(const HikCamera &) = delete;

    HikCamera(HikCamera &&) = delete;

    HikCamera &operator=(const HikCamera &) = delete;

    HikCamera &operator=(const HikCamera &&) = delete;

    ~HikCamera() = default;

    /**
     * \brief Open a camera.
     * \param [in] serial_number Serial number of the camera you wanna open.
     * \param [in] config_file Will load config from this file.
     * \return A boolean shows whether the camera is successfully opened.
     */
    bool OpenCamera(const std::string &serial_number, const std::string &config_file);

    /**
     * \brief Start the stream.
     * \return Whether stream is started normally.
     * \attention This function will return false when stream is already started or camera is not opened.
     */
    bool StartStream();

    /**
     * \brief Get an image from internal image buffer.
     * \param [out] image Acquired image will be stored here.
     * \return A boolean shows if buffer is not empty, or if you can successfully get an image.
     */
    bool GetImage(cv::Mat &image);

    /**
     * \brief Stop the stream.
     * \return Whether stream is stopped normally.
     * \attention This function will return false when stream is not started or camera is not opened.
     */
    bool StopStream();

    /**
     * \brief Close the opened camera.
     * \return A boolean shows whether the camera is normally closed.
     * \attention No matter what is returned, the camera will not be valid.
     */
    bool CloseCamera();

    /**
     * \brief Check if current device is connected.
     * \return A boolean shows current device is connected.
     */
    [[maybe_unused]] inline bool IsConnected() {
        if (device_ == nullptr) return false;
        return MV_CC_IsDeviceConnected(device_);
    }

    /**
     * \brief Export current config to specified file.
     * \param file_path File path.
     * \return A boolean shows if config is successfully saved.
     */
    inline bool ExportConfigurationFile(const std::string &file_path) {
        auto status_code = MV_CC_FeatureSave(device_, file_path.c_str());
        if (status_code != MV_OK) {
            LOG(INFO) << "Failed to save " << serial_number_ << "'s configuration to "
                      << file_path << " with error " << status_code << ".";
            return false;
        }
        LOG(INFO) << "Saved " << serial_number_ << "'s configuration to " << file_path << ".";
        return true;
    }

    /**
     * \brief Import current config to specified file.
     * \param file_path File path.
     * \return A boolean shows if config is successfully imported.
     */
    inline bool ImportConfigurationFile(const std::string &file_path) {
        auto status_code = MV_CC_FeatureLoad(device_, file_path.c_str());
        if (status_code != MV_OK) {
            LOG(INFO) << "Failed to save " << serial_number_ << "'s configuration to "
                      << file_path << " with error " << status_code << ".";
            return false;
        }
        LOG(INFO) << "Imported " << serial_number_ << "'s configuration to " << file_path << ".";
        return true;
    }

    [[maybe_unused]] inline std::string GetSerialNumber() { return serial_number_; }

    [[maybe_unused]] static inline unsigned int GetCameraCount() { return camera_count_; }

private:
    static unsigned int camera_count_;

    void *device_;

    bool stream_running_;
    bool stop_daemon_thread_flag_;
    pthread_t daemon_thread_id_;

    static void __stdcall ImageCallbackEx(unsigned char *, MV_FRAME_OUT_INFO_EX *, void *);

    static void *DaemonThreadFunction(void *);

    std::string serial_number_;

    Buffer<cv::Mat, 4> buffer_;
};

#endif  // _HIK_CAMERA_H_
