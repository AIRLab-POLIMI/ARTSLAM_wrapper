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
#ifndef ARTSLAM_GNSS_H
#define ARTSLAM_GNSS_H

#include "artslam_sensor.h"

// ROS messages
#include <sensor_msgs/NavSatFix.h>

// TF libraries
#include <tf/transform_listener.h>


namespace artslam
{
    namespace laser3d
    {
        /**
         * ARTSLAMGnss
         *
         * Sensor class casted to manage GNSS behaviour.
         */
        class ARTSLAMGnss : public ARTSLAMSensor<sensor_msgs::NavSatFixConstPtr>
        {
            private:
                tf::TransformListener tf_listener;

            public:
                ARTSLAMGnss()
                {
                    _prefilterer = false;
                    _tracker = false;
                    _ground_detector = false;
                    _topic = "/gnss_data"; // TODO: make it parametric
                    _buffer = 1024; // TODO: make it parametric
                };

                /**
                 * Method in charge to initialize the sensor ROS subscriber.
                 *
                 * @param mt_nh ROS Node Handler reference
                 */
                void setSubscriber(ros::NodeHandle* mt_nh)
                {
                    sensor_sub = mt_nh->subscribe(_topic, _buffer, &artslam::laser3d::ARTSLAMGnss::callback, this);
                };

                /**
                 * Sensor callback for the subscriber interaction.
                 *
                 * @param msg Template message which depends from the sensor type.
                 */
                void callback(const sensor_msgs::NavSatFixConstPtr& msg) override
                {
                    if(counter == 0)
                    {
                        tf::StampedTransform transform;
                        try
                        {
                            tf_listener.lookupTransform("base_link", msg->header.frame_id, ros::Time::now(), transform);
                        }
                        catch (std::exception &e)
                        {
                            std::cerr << "failed to find gps transform!!" << std::endl;
                            return;
                        }

                        EigVector3d g2l_translation(
                                transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
                        backend->backend_handler->set_gps_to_lidar_translation(g2l_translation);
                    }

                    GeoPointStamped_MSG::Ptr conv_gnss_msg(new GeoPointStamped_MSG);
                    conv_gnss_msg->header_.timestamp_ = msg->header.stamp.toNSec();
                    conv_gnss_msg->header_.frame_id_ = "base_link";
                    conv_gnss_msg->header_.sequence_ = counter;
                    counter++;
                    conv_gnss_msg->latitude_ = msg->latitude;
                    conv_gnss_msg->longitude_ = msg->longitude;
                    conv_gnss_msg->altitude_ = msg->altitude;
                    conv_gnss_msg->covariance_type_ = msg->position_covariance_type;

                    for(int i = 0; i < 9; i++)
                    {
                        if(conv_gnss_msg->covariance_type_ == 0)
                        {
                            conv_gnss_msg->covariance_[i] = 0.0;
                        }
                        else
                        {
                            conv_gnss_msg->covariance_[i] = msg->position_covariance[i];
                        }
                    }
                    backend->backend_handler->update_raw_gnss_observer(conv_gnss_msg);
                };
        };
    }
}

#endif // ARTSLAM_GNSS_H
