#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

namespace gazebo
{
    class Person : public ModelPlugin
    {

    private:
        int state;
        double vel;
        ignition::math::Pose3d pose;

    public:
        void Load(physics::ModelPtr _parent, sdf::ElementPtr)
        {
            this->model = _parent;
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                boost::bind(&Person::OnUpdate, this, _1));

            std::cout << "Loading person to follow at " << this->model->WorldPose() << std::endl;

            // Init values
            this->state = 0;
            this->vel = 0.5 + double(rand()) / RAND_MAX * 1.0;
        }

    public:
        void OnUpdate(const common::UpdateInfo &)
        {
            pose = this->model->WorldPose();
            pose.Rot().Euler(0, 0, 0);
            this->model->SetWorldPose(pose);
            this->model->SetAngularVel(ignition::math::Vector3d(0, 0, 0));

	    vel = 0.5 + double(rand()) / RAND_MAX * 1.0;
                // Init vels
                this->model->SetLinearVel(ignition::math::Vector3d(1 * vel, 0, 0));

        }

    private:
        physics::ModelPtr model;

    private:
        event::ConnectionPtr updateConnection;
    };
    GZ_REGISTER_MODEL_PLUGIN(Person)
}
