// -*- artslam-c++ -*-

/* ---------------------------------------------------------------------------------------------------------------------
 * Package: ARTSLAM-WRAPPER
 * Class: ARTSLAMLoopDetector
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
#include "kernel/artslam_loop_detector.h"

namespace artslam
{
    namespace laser3d
    {
        /**
         * Initialize loop detector sub-modules.
         *
         * @param config_file
         */
        void ARTSLAMLoopDetector::start(std::string config_file)
        {
            /* loop detector */
            Registration::Configuration loop_detector_registration_configuration =
                    parse_registration_loop_detector_configuration(config_file);
            Registration loop_detector_registration(loop_detector_registration_configuration);
            LoopDetector::Configuration loop_detector_configuration =
                    parse_loop_detector_configuration(config_file);
            loop_detector = std::make_shared<LoopDetector>(loop_detector_configuration,
                                                           loop_detector_registration.registration_method());
        }
    }
}
