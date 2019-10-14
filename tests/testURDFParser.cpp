#include <gtest/gtest.h>
#include <ModelInterfaceRBDL/ModelInterfaceRBDL.h>
#include <memory>
#include <urdf_parser/urdf_parser.h>
#include <boost/make_shared.hpp>


namespace{

class testURDFLoader: public ::testing::Test
{
public:
    testURDFLoader()
    {
        rbdl_model = std::make_shared<RigidBodyDynamics::Model>();
    }

    bool loadURDF(const std::string& urdf_file_path, const bool is_floating_base)
    {
        urdf_model = boost::make_shared<urdf::ModelInterface>();
        rbdl_model = std::make_shared<RigidBodyDynamics::Model>();

        urdf_model = urdf::parseURDFFile(urdf_file_path);
        if(urdf_model)
            return RigidBodyDynamics::Addons::URDFReadFromFile(urdf_file_path.c_str(), rbdl_model.get(), is_floating_base, false);
        return false;
    }

    std::string toRBDLJointType(const RigidBodyDynamics::JointType type)
    {
        if(type == 0)
            return"JointTypeUndefined";
        else if(type == 1)
            return "JointTypeRevolute";
        else if(type == 2)
            return "JointTypePrismatic";
        else if(type == 3)
            return "JointTypeRevoluteX";
        else if(type == 4)
            return "JointTypeRevoluteY";
        else if(type == 5)
            return "JointTypeRevoluteZ";
        else if(type == 6)
            return "JointTypeSpherical";
        else if(type == 7)
            return "JointTypeEulerZYX";
        else if(type == 8)
            return "JointTypeEulerXYZ";
        else if(type == 9)
            return "JointTypeEulerYXZ";
        else if(type == 10)
            return "JointTypeTranslationXYZ";
        else if(type == 11)
            return "JointTypeFloatingBase";
        else if(type == 12)
            return "JointTypeFixed";
        else if(type == 13)
            return "JointType1DoF";
        else if(type == 14)
            return "JointType2DoF";
        else if(type == 15)
            return "JointType3DoF";
        else if(type == 16)
            return "JointType4DoF";
        else if(type == 17)
            return "JointType5DoF";
        else if(type == 18)
            return "JointType6DoF";
        else if(type == 19)
            return "JointTypeCustom";
        else
            return "???";
    }

    virtual ~testURDFLoader() {

    }

    virtual void SetUp() {

    }

    virtual void TearDown() {

    }

    std::string path_to_models = MODEL_TESTS_ROBOTS_DIR;

    std::shared_ptr<RigidBodyDynamics::Model> rbdl_model;
    urdf::ModelInterfaceSharedPtr urdf_model;


private:

};

TEST_F(testURDFLoader, testURDFPlain)
{
    ///coman.urdf, floating_base = true
    std::string urdf_path =  this->path_to_models + "coman/coman.urdf";
    EXPECT_TRUE(this->loadURDF(urdf_path, true));

    unsigned int COMAN_DOFS = 29;
    unsigned int BASE_EULER_DOFS = 6;
    EXPECT_EQ(this->rbdl_model->q_size, COMAN_DOFS+BASE_EULER_DOFS);

    std::cout<<"coman.urdf"<<std::endl;
    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;
    unsigned int i = 0;
    for(RigidBodyDynamics::Joint joint : this->rbdl_model->mJoints)
    {
        std::cout<<"    JOINT "<<i<<"   TYPE: "<<toRBDLJointType(joint.mJointType)<<"     #DOFS: "<<joint.mDoFCount<<std::endl;
        ++i;
    }

    EXPECT_TRUE(this->urdf_model->getRoot()->name == "world");
    std::cout<<"    ROOT: "<<this->urdf_model->getRoot()->name<<std::endl;


    ///coman_no_world.urdf, floating_base = true
    urdf_path =  this->path_to_models + "coman/coman_no_world.urdf";
    EXPECT_TRUE(this->loadURDF(urdf_path, true));

    EXPECT_FALSE(this->rbdl_model->q_size == COMAN_DOFS+BASE_EULER_DOFS);

    std::cout<<"coman_no_world.urdf"<<std::endl;
    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;
    std::cout<<"NOTICE THAT EVEN IF THE MODEL IS LOADED IN RBDL WITH floating_base = true,"<<std::endl;
    std::cout<<"THE FLOATING BASE JOINTS ARE NOT PRESENT! THIS BECAUSE WE HAVE REMOVED THE world FRAME"<<std::endl;
    i = 0;
    for(RigidBodyDynamics::Joint joint : this->rbdl_model->mJoints)
    {
        std::cout<<"    JOINT "<<i<<"   TYPE: "<<toRBDLJointType(joint.mJointType)<<"     #DOFS: "<<joint.mDoFCount<<std::endl;
        ++i;
    }

    EXPECT_TRUE(this->urdf_model->getRoot()->name == "base_link");
    std::cout<<"    ROOT: "<<this->urdf_model->getRoot()->name<<std::endl;

    ///coman.urdf, floating_base = false
    urdf_path =  this->path_to_models + "coman/coman.urdf";
    EXPECT_TRUE(this->loadURDF(urdf_path, false));

    EXPECT_TRUE(this->rbdl_model->q_size == COMAN_DOFS+BASE_EULER_DOFS);

    std::cout<<"coman.urdf"<<std::endl;
    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;
    std::cout<<"NOTICE THAT THE MODEL IS LOADED IN RBDL WITH floating_base = false,"<<std::endl;
    std::cout<<"HOWEVER THE FLOATING BASE JOINTS ARE PRESENT! THIS BECAUSE WE HAVE THE world FRAME"<<std::endl;
    i = 0;
    for(RigidBodyDynamics::Joint joint : this->rbdl_model->mJoints)
    {
        std::cout<<"    JOINT "<<i<<"   TYPE: "<<toRBDLJointType(joint.mJointType)<<"     #DOFS: "<<joint.mDoFCount<<std::endl;
        ++i;
    }

    EXPECT_TRUE(this->urdf_model->getRoot()->name == "world");
    std::cout<<"    ROOT: "<<this->urdf_model->getRoot()->name<<std::endl;


    ///coman_no_base_link.urdf, floating_base = true
    urdf_path =  this->path_to_models + "coman/coman_no_base_link.urdf";
    EXPECT_TRUE(this->loadURDF(urdf_path, true));

    unsigned int BASE_QUATERNIONS = 7;
    EXPECT_TRUE(this->rbdl_model->q_size == COMAN_DOFS+BASE_QUATERNIONS);

    std::cout<<"coman_no_base_link.urdf"<<std::endl;
    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;

    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;
    std::cout<<"NOTICE THAT THE MODEL IS LOADED IN RBDL WITH floating_base = true,"<<std::endl;
    std::cout<<"THE ROOT OF THE MAODEL HAS INERTIA THEREFORE RBDL USES A SPHERICAL JOINT (QUATERNION)"<<std::endl;
    i = 0;
    for(RigidBodyDynamics::Joint joint : this->rbdl_model->mJoints)
    {
        std::cout<<"    JOINT "<<i<<"   TYPE: "<<toRBDLJointType(joint.mJointType)<<"     #DOFS: "<<joint.mDoFCount<<std::endl;
        ++i;
    }

    EXPECT_TRUE(this->urdf_model->getRoot()->name == "Waist");
    std::cout<<"    ROOT: "<<this->urdf_model->getRoot()->name<<std::endl;


    ///coman_no_base_link.urdf, floating_base = false
    urdf_path =  this->path_to_models + "coman/coman_no_base_link.urdf";
    EXPECT_TRUE(this->loadURDF(urdf_path, false));

    EXPECT_FALSE(this->rbdl_model->q_size == COMAN_DOFS+BASE_QUATERNIONS);

    std::cout<<"coman_no_base_link.urdf"<<std::endl;
    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;

    std::cout<<"    #JOINTS: "<<this->rbdl_model->q_size<<std::endl;
    std::cout<<"NOTICE THAT THE MODEL IS LOADED IN RBDL WITH floating_base = false,"<<std::endl;
    std::cout<<"IN THIS CASE THE FLAG IS TAKEN INTO ACCOUNT"<<std::endl;
    i = 0;
    for(RigidBodyDynamics::Joint joint : this->rbdl_model->mJoints)
    {
        std::cout<<"    JOINT "<<i<<"   TYPE: "<<toRBDLJointType(joint.mJointType)<<"     #DOFS: "<<joint.mDoFCount<<std::endl;
        ++i;
    }

    EXPECT_TRUE(this->urdf_model->getRoot()->name == "Waist");
    std::cout<<"    ROOT: "<<this->urdf_model->getRoot()->name<<std::endl;


}

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
