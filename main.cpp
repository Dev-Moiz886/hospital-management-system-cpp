#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

//Check for integers
int getValidInt(const string& message)
{
    int value;
    while(true)
    {
        cout << message;
        if(cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout<<"Invalid Input! Numbers Only.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

//Chech for gender
string getValidGender()
{
    string gender;
    while(true)
    {
        cout<<"Enter Gender (Male / Female / Other): ";
        getline(cin, gender);

        if(gender == "Male"||gender == "Female"||gender == "Other")
            return gender;

        cout<<"Invalid Gender! Enter Male, Female, or Other Only.\n";
    }
}

//Abstraction
class IFileHandler
{
public:
    virtual void saveToFile()   const = 0;
    virtual void loadFromFile()       = 0;
    virtual ~IFileHandler(){
	}
};

class Person
{
protected:
    int id;
    string name;
    int age;
    string gender;

public:
    Person(int i, string n, int a, string g):id(i), name(n), age(a), gender(g){
		}

    virtual void display()const = 0;  
    virtual string serialize()const = 0;  

    int getID()const{
		return id;     
		}
    string getName()const{
		return name;   
		}
    int getAge() const{
		return age;
	    }
    string getGender() const{
		return gender;
		}

    virtual ~Person(){
	}
};


class Patient:public Person
{
    string disease;

public:
    Patient(int i, string n, int a, string g, string d) : Person(i, n, a, g), disease(d){
	}

    string getDisease()const{
		return disease;
		}

    void display() const override
    {
        cout<<"\n--- PATIENT ---"<<"\nID       : "<<id<<"\nName     : "<<name<<"\nAge      : "<<age<<"\nGender   : "<<gender<<"\nDisease  : "<<disease <<"\n";
    }

    string serialize() const override
    {
        return "PATIENT|" + to_string(id) + "|" + name + "|"
             + to_string(age) + "|" + gender + "|" + disease;
    }

    static Patient deserialize(const string& line)
    {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|'))
            parts.push_back(token);

        return Patient(stoi(parts[1]), parts[2], stoi(parts[3]), parts[4], parts[5]);
    }
};


class Doctor:public Person
{
    string specialization;

public:
    Doctor(int i, string n, int a, string g, string s) : Person(i, n, a, g), specialization(s){
	}

    string getSpecialization()const{
		return specialization;
		}

    void display() const override
    {
        cout<<"\n--- DOCTOR ---"<<"\nID             : "<<id<<"\nName           : "<<name<<"\nAge            : "<<age<<"\nGender         : "<<gender<<"\nSpecialization : "<<specialization<<"\n";
    }

    string serialize() const override
    {
        return "DOCTOR|" + to_string(id) + "|" + name + "|"
             + to_string(age) + "|" + gender + "|" + specialization;
    }

    static Doctor deserialize(const string& line)
    {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|'))
            parts.push_back(token);
        return Doctor(stoi(parts[1]), parts[2], stoi(parts[3]), parts[4], parts[5]);
    }
};


class Employee:public Person
{
    string designation;

public:
    Employee(int i, string n, int a, string g, string d)
        : Person(i, n, a, g), designation(d) {}

    string getDesignation() const { return designation; }

    void display() const override
    {
        cout<<"\n--- EMPLOYEE ---"<<"\nID          : "<<id<<"\nName        : "<<name<<"\nAge         : "<<age<<"\nGender      : "<<gender<<"\nDesignation : "<<designation<<"\n";
    }

    string serialize() const override
    {
        return "EMPLOYEE|" + to_string(id) + "|" + name + "|"
             + to_string(age) + "|" + gender + "|" + designation;
    }

    static Employee deserialize(const string& line)
    {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|'))
            parts.push_back(token);
        return Employee(stoi(parts[1]), parts[2], stoi(parts[3]), parts[4], parts[5]);
    }
};


class Appointment
{
    int appointmentID;
    int patientID;
    int doctorID;
    string date;

public:
    Appointment(int aID, int pID, int dID, string dt)
        : appointmentID(aID), patientID(pID), doctorID(dID), date(dt) {}

    int getAppointmentID()const{
		return appointmentID;
		}

    void display()const
    {
        cout<<"\n--- APPOINTMENT ---"<<"\nAppointment ID : "<<appointmentID<<"\nPatient ID     : "<<patientID<<"\nDoctor ID      : "<<doctorID<<"\nDate           : "<<date<<"\n";
    }

    string serialize() const
    {
        return "APPOINTMENT|" + to_string(appointmentID) + "|"
             + to_string(patientID) + "|" + to_string(doctorID) + "|" + date;
    }

    static Appointment deserialize(const string& line)
    {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|'))
            parts.push_back(token);
        return Appointment(stoi(parts[1]), stoi(parts[2]), stoi(parts[3]), parts[4]);
    }
};


class Hospital:public IFileHandler
{
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Employee> employees;
    vector<Appointment> appointments;

    const string FILE_PATIENTS = "patients.txt";
    const string FILE_DOCTORS = "doctors.txt";
    const string FILE_EMPLOYEES = "employees.txt";
    const string FILE_APPOINTMENTS = "appointments.txt";


    bool patientExists(int id) const
    {
        for(const auto& p : patients)
            if(p.getID() == id){
			return true;
			}
        return false;
    }

    bool doctorExists(int id) const
    {
        for(const auto& d : doctors)
            if(d.getID() == id){
			return true;
			}	
        return false;
    }

    bool employeeExists(int id) const 
    {
        for(const auto& e : employees)
            if(e.getID() == id){
			return true;
			}
        return false;
    }

    bool appointmentExists(int id) const 
    {
        for(const auto& a : appointments)
            if(a.getAppointmentID() == id){
			return true;
			}
        return false;
    }

    int getValidAge()
    {
        int age;
        do{
            age = getValidInt("Enter Age: ");
            if(age<0 || age>120)
                cout<<"Invalid Age! Enter Between 0-120.\n";
        }while(age<0 ||age>120);
        return age;
    }

public:
	
	//For auto load on run
    Hospital(){
		loadFromFile();
		}
	//For Auto save at last
    ~Hospital(){
		saveToFile();
		} 


    void saveToFile() const override
    {
        // Patients data saving
        ofstream fp(FILE_PATIENTS);
        if(!fp){
			cout<<"Error: Could Not Save Patients!\n";
			}
        else
        {
            for(const auto& p : patients)
                fp<<p.serialize()<<"\n";
            fp.close();
        }

        // Doctors data saving
        ofstream fd(FILE_DOCTORS);
        if(!fd){
			cout<<"Error: Could Not Save Doctors!\n";
			}
        else
        {
            for(const auto& d : doctors)
                fd<<d.serialize()<<"\n";
            fd.close();
        }

        // Employees data saving
        ofstream fe(FILE_EMPLOYEES);
        if(!fe){
			cout<<"Error: Could Not Save Employees!\n";
			}
        else
        {
            for(const auto& e : employees)
                fe<<e.serialize()<<"\n";
            fe.close();
        }

        // Appointments data saving
        ofstream fa(FILE_APPOINTMENTS);
        if(!fa){
			cout<<"Error: Could Not Save Appointments!\n";
			}
        else
        {
            for(const auto& a : appointments)
                fa<<a.serialize()<<"\n";
            fa.close();
        }

        cout<<"All Records Saved Successfully!\n";
    }

    void loadFromFile() override
    {
        string line;

        // showing Patients
        ifstream fp(FILE_PATIENTS);
        if(fp)
        {
            while(getline(fp, line))
                if(!line.empty())
					patients.push_back(Patient::deserialize(line));
            fp.close();
        }

        // showing Doctors
        ifstream fd(FILE_DOCTORS);
        if(fd)
        {
            while(getline(fd, line))
                if(!line.empty())
					doctors.push_back(Doctor::deserialize(line));
            fd.close();
        }

        //showing Employees
        ifstream fe(FILE_EMPLOYEES);
        if(fe)
        {
            while(getline(fe, line))
                if(!line.empty())
					employees.push_back(Employee::deserialize(line));
            fe.close();
        }

        // showing Appointments
        ifstream fa(FILE_APPOINTMENTS);
        if (fa)
        {
            while(getline(fa, line))
                if(!line.empty())
					appointments.push_back(Appointment::deserialize(line));
            fa.close();
        }

        cout<<"Records Loaded Successfully!\n";
    }

//For adding
    void addPatient()
    {
        int id = getValidInt("Enter Patient ID: ");
        if(patientExists(id)){
			cout<<"Patient ID Already Exists!\n";
			return;
			}

        string name, gender, disease;
        cout<<"Enter Name: ";
		getline(cin, name);
        int age=getValidAge();
        gender=getValidGender();
        cout<<"Enter Disease: ";
		getline(cin, disease);

        patients.push_back(Patient(id, name, age, gender, disease));
        cout<<"Patient Added Successfully!\n";
    }

    void addDoctor()
    {
        int id = getValidInt("Enter Doctor ID: ");
        if(doctorExists(id)){
			cout<<"Doctor ID Already Exists!\n";
			return;
			}

        string name, gender, spec;
        cout<<"Enter Name: ";
		getline(cin, name);
        int age = getValidAge();
        gender = getValidGender();
        cout<<"Enter Specialization: ";
		getline(cin, spec);

        doctors.push_back(Doctor(id, name, age, gender, spec));
        cout<<"Doctor Added Successfully!\n";
    }

    void addEmployee()
    {
        int id = getValidInt("Enter Employee ID: ");
        if (employeeExists(id)){
			cout<<"Employee ID Already Exists!\n";
			return;
			}

        string name, gender, desig;
        cout<<"Enter Name: ";
		getline(cin, name);
        int age = getValidAge();
        gender = getValidGender();
        cout<<"Enter Designation: ";
		getline(cin, desig);

        employees.push_back(Employee(id, name, age, gender, desig));
        cout<<"Employee Added Successfully!\n";
    }

    void makeAppointment()
    {
        int appID = getValidInt("Enter Appointment ID: ");
        if(appointmentExists(appID)){
			cout<<"Appointment ID Already Exists!\n";
			return;
			}

        int patientID = getValidInt("Enter Patient ID: ");
        if(!patientExists(patientID)){
			cout<<"Patient Does Not Exist!\n";
			return;
			}

        int doctorID = getValidInt("Enter Doctor ID: ");
        if (!doctorExists(doctorID)){
			cout<<"Doctor Does Not Exist!\n";
			return;
			}

        string date;
        cout<<"Enter Date (DD-MM-YYYY): ";
        getline(cin, date);

        appointments.push_back(Appointment(appID, patientID, doctorID, date));
        cout<<"Appointment Created Successfully!\n";
    }

// For searching by id
    void searchPatient()
    {
        int id = getValidInt("Enter Patient ID: ");
        for(const auto& p : patients)
            if(p.getID() == id){
				p.display();
				return;
				}
        cout<<"Patient Not Found!\n";
    }

    void searchDoctor()
    {
        int id = getValidInt("Enter Doctor ID: ");
        for(const auto& d : doctors)
            if(d.getID() == id){
				d.display();
				return;
				}
        cout<<"Doctor Not Found!\n";
    }

// For searching by name
    void searchPatientByName()
    {
        string name;
        cout<<"Enter Patient Name: ";
        getline(cin, name);

        bool found = false;
        for(const auto& p : patients)
        {
            if(p.getName() == name)
            {
                p.display();
                found = true;
            }
        }
        if(!found)
			cout<<"No Patient Found With Name: "<<name<<"\n";
    }

    void searchDoctorByName()
    {
        string name;
        cout<<"Enter Doctor Name: ";
        getline(cin, name);

        bool found = false;
        for(const auto& d : doctors)
        {
            if(d.getName() == name)
            {
                d.display();
                found = true;
            }
        }
        if(!found)
			cout<<"No Doctor Found With Name: "<<name<<"\n";
    }

//For updating
    void updatePatient()
    {
        int id=getValidInt("Enter Patient ID To Update: ");

        for(auto& p : patients)
        {
            if(p.getID() == id)
            {
                cout<<"--- Current Record ---";
                p.display();

                string name, disease;
                cout<<"\nEnter New Name: ";
				getline(cin, name);
                int age  = getValidAge();
                string gender = getValidGender();
                cout<<"Enter New Disease: ";
				getline(cin, disease);

                p=Patient(id, name, age, gender, disease);
                cout<<"Patient Updated Successfully!\n";
                return;
            }
        }
        cout<<"Patient Not Found!\n";
    }

    void updateDoctor()
    {
        int id=getValidInt("Enter Doctor ID To Update: ");

        for(auto& d : doctors)
        {
            if(d.getID() == id)
            {
                cout<<"--- Current Record ---";
                d.display();

                string name, spec;
                cout<<"\nEnter New Name: ";
				getline(cin, name);
                int age  = getValidAge();
                string gender = getValidGender();
                cout<<"Enter New Specialization: ";
				getline(cin, spec);

                d=Doctor(id, name, age, gender, spec);
                cout<<"Doctor Updated Successfully!\n";
                return;
            }
        }
        cout<<"Doctor Not Found!\n";
    }

    void updateEmployee()
    {
        int id = getValidInt("Enter Employee ID To Update: ");

        for(auto& e : employees)
        {
            if(e.getID() == id)
            {
                cout<<"--- Current Record ---";
                e.display();

                string name, desig;
                cout<<"\nEnter New Name: ";
				getline(cin, name);
                int age  = getValidAge();
                string gender = getValidGender();
                cout<<"Enter New Designation: ";
				getline(cin, desig);

                e=Employee(id, name, age, gender, desig);
                cout<<"Employee Updated Successfully!\n";
                return;
            }
        }
        cout<<"Employee Not Found!\n";
    }

//For deleting
    void dischargePatient()
    {
        int id = getValidInt("Enter Patient ID To Discharge: ");
        for(auto it = patients.begin(); it != patients.end(); ++it)
        {
            if(it->getID() == id)
            {
                patients.erase(it);
                cout<<"Patient Discharged Successfully!\n";
                return;
            }
        }
        cout<<"Patient Not Found!\n";
    }

    void cancelAppointment()
    {
        int id = getValidInt("Enter Appointment ID: ");
        for (auto it = appointments.begin(); it != appointments.end(); ++it)
        {
            if(it->getAppointmentID() == id)
            {
                appointments.erase(it);
                cout<<"Appointment Cancelled Successfully!\n";
                return;
            }
        }
        cout<<"Appointment Not Found!\n";
    }

//For showing
    void showPatients()
    {
        if(patients.empty()){
			cout<<"No Patients Found!\n"; return;
			}
        for (const auto& p : patients)
			p.display();
    }

    void showDoctors()
    {
        if(doctors.empty()){
			cout<<"No Doctors Found!\n"; return;}
        for (const auto& d : doctors)
			d.display();
    }

    void showEmployees()
    {
        if (employees.empty()){
			cout<<"No Employees Found!\n"; return;}
        for (const auto& e : employees)
			e.display();
    }

    void showAppointments()
    {
        if(appointments.empty()){
			cout<<"No Appointments Found!\n"; return;
			}
        for(const auto& a : appointments)
			a.display();
    }
};



int main()
{
    Hospital h;
    int choice;

    do
    {
        cout<<"\n======= HOSPITAL MANAGEMENT SYSTEM =======\n";
        cout<<"\n-------------- PATIENT --------------------\n";
        cout<<" 1.  Add Patient\n";
        cout<<" 2.  Discharge Patient\n";
        cout<<" 3.  Search Patient (By ID)\n";
        cout<<" 4.  Search Patient (By Name)\n";
        cout<<" 5.  Update Patient\n";
        cout<<" 6.  Show All Patients\n";
        cout<<"\n-------------- DOCTOR ---------------------\n";
        cout<<" 7.  Add Doctor\n";
        cout<<" 8.  Search Doctor  (By ID)\n";
        cout<<" 9.  Search Doctor  (By Name)\n";
        cout<<" 10. Update Doctor\n";
        cout<<" 11. Show All Doctors\n";
        cout<<"\n-------------- EMPLOYEE -------------------\n";
        cout<<" 12. Add Employee\n";
        cout<<" 13. Update Employee\n";
        cout<<" 14. Show All Employees\n";
        cout<<"\n-------------- APPOINTMENT ----------------\n";
        cout<<" 15. Make Appointment\n";
        cout<<" 16. Cancel Appointment\n";
        cout<<" 17. Show All Appointments\n";
        cout<<"\n-------------- OTHER ----------------------\n";
        cout<<" 18. Save Records Manually\n";
        cout<<" 19. Exit\n";
        cout<<"==========================================\n";

        choice = getValidInt("Enter Choice: ");

        switch (choice)
        {
        case 1:
			h.addPatient();
			break;
        case 2:
			h.dischargePatient();
			break;
        case 3:
			h.searchPatient();
			break;
        case 4:
			h.searchPatientByName();
			break;
        case 5:
			h.updatePatient();
			break;
        case 6:
			h.showPatients();
			break;
        case 7:
			h.addDoctor();
			break;
        case 8:
			h.searchDoctor();
			break;
        case 9:
			h.searchDoctorByName();
			break;
        case 10:
			h.updateDoctor();
			break;
        case 11:	
			h.showDoctors();
			break;
        case 12:
			h.addEmployee();
			break;
        case 13:
			h.updateEmployee();
			break;
        case 14:
			h.showEmployees();
			break;
        case 15:
			h.makeAppointment();
			break;
        case 16:
			h.cancelAppointment();
			break;
        case 17:
			h.showAppointments();
			break;
        case 18:
			h.saveToFile();
			break;
        case 19:
			cout << "Exiting... Records Saved!\n";
			break;
        default:
			cout << "Invalid Choice!\n";
        }

    } while (choice != 19);

    return 0;
}