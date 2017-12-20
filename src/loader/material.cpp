#include"material.h"

MaterialLoader::MaterialLoader() {}

void MaterialLoader::load(std::string filename) {
    std::ifstream inFile;

    material_t material;

    std::string folder("models/");

    inFile.open(folder + filename);
    if(!inFile) {
        std::cout << "Can't open material file: " << filename << std::endl;
        exit(1);
    }

    std::string ch;
    while(inFile >> ch) {

        if(ch == "newmtl") {
            if(material.name != "") {
                this->materials.push_back(material);
            }

            inFile >> material.name;

            //fscanf(fp, "%s\n", material.name);

        } else if(ch == "Ns") {
            //fscanf(fp, "%f\n", &material.exponent);
            inFile >> material.exponent;

        } else if(ch == "Ka") {
            //fscanf(fp, "%f %f %f\n", &material.ambient[0],&material.ambient[1],&material.ambient[2]);
            inFile >> material.ambient[0];
            inFile >> material.ambient[1];
            inFile >> material.ambient[2];
            material.ambient[3] = 1.0;

        } else if(ch == "Kd") {
            //fscanf(fp, "%f %f %f\n", &material.diffuse[0],&material.diffuse[1],&material.diffuse[2]);
            inFile >> material.diffuse.x;
            inFile >> material.diffuse.y;
            inFile >> material.diffuse.z;
            material.diffuse.w = 1.0;
        } else if(ch == "Ks") {
            //fscanf(fp, "%f %f %f\n", &material.specular[0],&material.specular[1],&material.specular[2]);
            inFile >> material.specular[0];
            inFile >> material.specular[1];
            inFile >> material.specular[2];
            material.specular[3] = 1.0;
        } else if(ch == "d") {
            //fscanf(fp, "%f\n", &material.alpha);
            inFile >> material.alpha;
        } else if(ch == "illum") {
            //fscanf(fp, "%d\n", &material.illum);
            inFile >> material.illum;
        }
    } // Done reading file*/

    // Push last material
    this->materials.push_back(material);

    inFile.close();
}

material_t MaterialLoader::get(std::string name) {
    for(unsigned int i = 0; i < this->materials.size(); i++) {
        if(this->materials[i].name == name) {
            return this->materials[i];
        }
    }
    return this->materials[0];
}
