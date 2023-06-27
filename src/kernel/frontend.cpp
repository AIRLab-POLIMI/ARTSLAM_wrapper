// -*- lots-wrapper-c++ -*-

/* ---------------------------------------------------------------------------------------------------------------------
 * Package: LOTS-WRAPPER
 * Class: Frontend
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
#include "kernel/frontend.hpp"

namespace lots::slam::wrapper
        {
    using namespace lots::slam::utils;
            /**
             * Initialize front-end sub-modules based on flag configuration which strongly depends from the sensor type.
             *
             * @param config_file Configuration file.
             * @param sensor Name of the sensor.
             * @param id Id of the sensor.
             */
            void Frontend::start(std::string config_file, SensorType sensor, int id)
            {
                std::vector<std::string> module_list = parse_frontend_module_list(config_file, sensor, id);

                for (std::string module_name : module_list)
                {
                    if (module_name == "prefilterer")
                        addPrefilterer(config_file, sensor, id);
                    else if (module_name == "ground_detector")
                        addGroundDetector(config_file, sensor, id);
                    else if (module_name == "tracker")
                        addTracker(config_file, sensor, id);
                    else if (module_name == "registration_tracker")
                        continue;
                    else
                        std::cout << "[ERROR] " << module_name << " module does not exist!" << std::endl;
                }
            }

            /**
             * Add a prefilterer module.
             *
             * @param config_file Configuration file.
             * @param sensor Name of the sensor.
             * @param id Id of the sensor.
             */
            void Frontend::addPrefilterer(std::string config_file, SensorType sensor, int id)
            {
                std::cout << ">> PRE-FILTERER" << std::endl;
                LiDARPrefilterer::Configuration config = parse_prefilterer_configuration(config_file, sensor, id);

                if (sensor == LIDAR) {
                    modules["prefilterer"] = std::make_shared<LiDARPrefilterer>(config);
                    modules["prefilterer"]->set_id(LIDAR, id, 0, 0);
                } else if (sensor == CAMERA)
                    std::cout << "[ERROR] Camera Prefilterer not yet implemented!" << std::endl;
                    //TODO: modules["prefilterer"] = std::make_shared<CameraPrefilterer>(config);
                    //modules["prefilterer"]->set_id("camera", id);
                else if (sensor == RADAR)
                    std::cout << "[ERROR] Radar Prefilterer not yet implemented!" << std::endl;
                    //TODO: modules["prefilterer"] = std::make_shared<RadarPrefilterer>(config);
                    //modules["prefilterer"]->set_id("radar", id);
                else
                    std::cout << "[ERROR] Prefilterer module for " << sensor << " sensor does not exist!" << std::endl;

                std::cout << std::endl;
            }

            /**
             * Add a tracker module.
             *
             * @param config_file Configuration file.
             * @param sensor Name of the sensor.
             * @param id Id of the sensor.
             */
            void Frontend::addTracker(std::string config_file, SensorType sensor, int id)
            {
                std::cout << ">> TRACKER" << std::endl;
                LiDARTracker::Configuration config = parse_tracker_configuration(config_file, sensor, id);

                if (sensor == LIDAR) {
                    LiDARRegistration::Configuration reg_config = parse_registration_tracker_configuration(config_file, sensor, id);
                    LiDARRegistration registration(reg_config);
                    modules["tracker"] = std::make_shared<LiDARTracker>(config, registration.registration_method());
                    modules["tracker"]->set_id(LIDAR, id, 0, 0);
                    if (modules.find("prefilterer") != modules.end())
                        (static_cast<LiDARPrefilterer*>(modules["prefilterer"].get()))->register_point_cloud_observer(static_cast<LiDARTracker*>(modules["tracker"].get()));
                } else if (sensor == CAMERA)
                    std::cout << "[ERROR] Camera Tracker not yet implemented!" << std::endl;
                    //TODO: modules["tracker"] = std::make_shared<CameraTracker>(config);
                    //modules["tracker"]->set_id("camera", id);
                else if (sensor == RADAR)
                    std::cout << "[ERROR] Radar Tracker not yet implemented!" << std::endl;
                    //TODO: modules["tracker"] = std::make_shared<RadarTracker>(config);
                    //modules["tracker"]->set_id("radar", id);
                else if (sensor == ODOMETRY) {
                    std::cout << "[ERROR] Odometry Tracker not yet implemented!" << std::endl;
//                    modules["tracker"] = std::make_shared<OdometryTracker>(config);
//                    modules["tracker"]->set_id("odom", id);
                } else
                    std::cout << "[ERROR] Tracker module for " << sensor << " sensor does not exist!" << std::endl;

                std::cout << std::endl;
            }

            /**
             * Add a ground detector module.
             *
             * @param config_file Configuration file.
             * @param sensor Name of the sensor.
             * @param id Id of the sensor.
             */
            void Frontend::addGroundDetector(std::string config_file, SensorType sensor, int id)
            {
                std::cout << ">> GROUND-DETECTOR" << std::endl;
                LiDARGroundDetector::Configuration config = parse_ground_detector_configuration(config_file, sensor, id);
                if (sensor == LIDAR) {
                    modules["ground_detector"] = std::make_shared<LiDARGroundDetector>(config);
                    modules["ground_detector"]->set_id(LIDAR, id, 0,0);
                    if (modules.find("prefilterer") != modules.end())
                        (static_cast<LiDARPrefilterer*>(modules["prefilterer"].get()))->register_point_cloud_observer(static_cast<LiDARGroundDetector*>(modules["ground_detector"].get()));
                } else if (sensor == CAMERA)
                    std::cout << "[ERROR] Camera Ground Detector not yet implemented!" << std::endl;
                    //TODO: modules["ground_detector"] = std::make_shared<CameraGroundDetector>(config);
                    //modules["ground_detector"]->set_id("camera", id);
                else
                    std::cout << "[ERROR] Ground Detector module for " << sensor << " sensor does not exist!" << std::endl;

                std::cout << std::endl;
            }
        }
