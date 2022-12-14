// -*- artslam-c++ -*-

/* ---------------------------------------------------------------------------------------------------------------------
 * Package: ARTSLAM-WRAPPER
 * Class: ARTSLAMController
 * Author: Mirko Usuelli
 * Advisor: Prof. Matteo Matteucci, PhD
 * Co-Advisors: Matteo Frosi, PhD; Gianluca Bardaro, PhD; Simone Mentasti, PhD; Paolo Cudrano, PhD Student.
 * University: Politecnico di Milano - Artificial Intelligence & Robotics Lab
 * ---------------------------------------------------------------------------------------------------------------------
 * This file is part of {{ ARTSLAM_WRAPPER }}.
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
#ifndef ARTSLAM_LIDAR_H
#define ARTSLAM_LIDAR_H

#include "artslam_sensor.h"

// ROS messages
#include <sensor_msgs/PointCloud2.h>

// PCL libraries
#include <pcl_conversions/pcl_conversions.h>


namespace artslam
{
    namespace laser3d
    {
        /**
         * ARTSLAMLidar
         *
         * ...
         */
        class ARTSLAMLidar : public ARTSLAMSensor<sensor_msgs::PointCloud2ConstPtr>
        {
            public:
                ARTSLAMLidar()
                {
                    _prefilterer = true;
                    _tracker = true;
                    _ground_detector = true;
                    _topic = "/velodyne_points"; // TODO: make it parametric
                    _buffer = 64; // TODO: make it parametric
                };

                /**
                 * Method in charge to initialize the sensor ROS subscriber.
                 *
                 * @param mt_nh ROS Node Handler reference
                 */
                void setSubscriber(ros::NodeHandle* mt_nh)
                {
                    sensor_sub = mt_nh->subscribe(_topic, _buffer, &artslam::laser3d::ARTSLAMLidar::callback, this);
                };

                /**
                 * Sensor callback for the subscriber interaction.
                 *
                 * @param msg Template message which depends from the sensor type.
                 */
                void callback(const sensor_msgs::PointCloud2ConstPtr& msg) override
                {
                    if(!ros::ok())
                        return;

                    pcl::PointCloud<Point3I>::Ptr cloud(new pcl::PointCloud<Point3I>());
                    pcl::fromROSMsg(*msg, *cloud);
                    cloud->header.seq = counter;

                    cloud->header.stamp = msg->header.stamp.toNSec();
                    frontend.prefilterer->update_raw_pointcloud_observer(cloud);

                    counter++;
                };
        };
    }
}

#endif // ARTSLAM_LIDAR_H
