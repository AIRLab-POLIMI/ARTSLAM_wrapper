// -*- lots-wrapper-c++ -*-

/* ---------------------------------------------------------------------------------------------------------------------
 * Package: LOTS-WRAPPER
 * Class: Camera
 * Author: Mirko Usuelli
 * Advisor: Prof. Matteo Matteucci, PhD
 * Co-Advisors: Matteo Frosi, PhD; Gianluca Bardaro, PhD; Simone Mentasti, PhD; Paolo Cudrano, PhD Student.
 * University: Politecnico di Milano - Artificial Intelligence & Robotics Lab
 * ---------------------------------------------------------------------------------------------------------------------
 * This file is part of {{ LOTS-WRAPPER }}.
 *
 * Developed for the Politecnico di Milano (Artificial Intelligence & Robotics Lab)
 * This product includes software developed by Matteo Frosi. See the README file at the top-level directory of this
 * distribution for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify it.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * ---------------------------------------------------------------------------------------------------------------------
 */
#ifndef WRAPPER_CAMERA_H
#define WRAPPER_CAMERA_H

#include "sensor.hpp"

// ROS messages
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <nav_msgs/msg/odometry.hpp>

// PCL libraries
#include <pcl_conversions/pcl_conversions.h>
#include <tf2_eigen/tf2_eigen.hpp>

namespace artslam::lots::wrapper {
    /**
     * Camera
     *
     * ...
     */
    class Camera : public Sensor<sensor_msgs::msg::Image>  // TODO: change msg type
    {
    public:
        explicit Camera(int id) {
            _sensor_type = "CAMERA";
            _sensor_id = id;

            _start_color = "\033[1;31m";
            _end_color = "\033[0m";

//            _topic = topic;
//            _buffer = buffer;
        };

        /**
         * Method in charge to initialize the sensor ROS subscriber.
         *
         * @param n ROS Node reference
         */
//        void setSubscribers(std::shared_ptr<rclcpp::Node> n) {
//            // commented out, since the callback does nothing
//            //sensor_sub = n->subscribe(_topic, _buffer, &artslam::lots::wrapper::Camera::callback, this);
//        };

        /**
         * Sensor callback for the subscriber interaction.
         *
         * @param msg Template message which depends from the sensor type.
         */

        void callback(const sensor_msgs::msg::Image& msg) override
        {};

        /**
         * Sensor callback for the prior odom topic stram.
         *
         * @param msg Odometry message.
         */

        void prior_odom_callback(const nav_msgs::msg::Odometry &msg) override {
            /*
            tf2::Pose tf_pose;
            Eigen::Isometry3d i3d;
            tf2::poseMsgToTF(msg->pose.pose, tf_pose);
            tf2::poseTFToEigen(tf_pose, i3d);

            OdometryStamped3D_MSG::Ptr odom_msg(new OdometryStamped3D_MSG());
            odom_msg->header_.timestamp_ = msg.header.stamp.toNSec();
            odom_msg->header_.frame_id_ = "base_link";
            odom_msg->header_.sensor_type_ = boost::algorithm::to_lower_copy(_sensor_type);;
            odom_msg->header_.sensor_id_ = _sensor_id;
            odom_msg->odometry_ = i3d.matrix().cast<float>();
            odom_msg->covariance_ = Eigen::MatrixXd::Map(&(msg.pose.covariance[0]), 6, 6);

            //TODO: uncomment it when it is ready
            //(static_cast<CameraTracker*>(frontend.modules["tracker"].get()))->update_prior_odometry_observer(odom_msg);
            */
        };
    };
}

#endif // WRAPPER_CAMERA_H
