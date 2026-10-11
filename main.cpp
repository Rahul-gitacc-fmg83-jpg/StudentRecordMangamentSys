#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<algorithm>
#include<cstdint>
#include<ctime>
#include<cstdio>
#define line "-------------------------------------------------------------------------------\n";
class Sorting;
class Student{
    public:
        Student(void){}

    private:
        int id;
        int semester;
        std::string name;
        std::string dob;
        std::string course;
        float marks;

    public:
        void setData(void);
        void putData(void);
        void save(std::ostream& out) const;
        void load(std::istream& in);

    public:
        friend class Record;
        friend class Sorting;
        template<class T>
        friend void swap(T &a,T &b){
            T temp=a;
            a=b;
            b=temp;
        }
        friend void logHistory(const std::string& action,const std::string& details);
        friend void showHistory();
        friend void clearHisoty();
        
        

        
};
class Record{
    public:
        void showRecords(std::vector<Student> &vec);
        void addRecord(std::vector<Student>& vec);
        void deleteRecord(std::vector<Student>& vec);
        void updateRecord(std::vector<Student>& vec);
        void searchRecord(std::vector<Student>& vec);
        void searchSimilarTypeRecord(std::vector<Student>& vec);
        void sortingRecord(std::vector<Student>& vec);
        void showTopperRecords(std::vector<Student> & vec);
};
class Sorting{
    public:
        std::vector<Student> ByName(std::vector<Student> vec,std::string order);
        std::vector<Student> ById(std::vector<Student> vec,std::string order);      
        std::vector<Student> ByMarks(std::vector<Student> vec,std::string order);      
        std::vector<Student> BySemester(std::vector<Student> vec,std::string order);      
        std::vector<Student> ByCourse(std::vector<Student> vec,std::string order);      
        std::vector<Student> ByDob(std::vector<Student> vec,std::string order);      
};
void logHistory(const std::string& action, const std::string& details){
        std::time_t t=std::time(nullptr);
        char buf[32];
        std::strftime(buf,sizeof(buf),"%d-%m-%Y% %H:%M:%S",std::localtime(&t));
        std::string when=buf;

        std::ofstream fout("history.bin",std::ios::binary|std::ios::app);
        auto wstr=[&](const std::string& s){
            std::uint64_t len= s.size();
            fout.write(reinterpret_cast<const char*>(&len),sizeof(len));
            fout.write(s.data(),len);
        };
        wstr(action); wstr(when); wstr(details);
}
void clearHistory(){
    char ch;
    std::cout<<"!!! Permanently clear all history !!! y/n: ";
    std::cin>>ch;
    if(ch=='y'|| ch=='Y'){
        std::ofstream fout("history.bin",std::ios::binary|std::ios::trunc);
        std::cout<<line;
        std::cout<<"\t\tHistory CLEARED Successfully\n";
        std::cout<<line;
        fout.close();
    
    }
}
void showHistory(){
    std::ifstream fin("history.bin",std::ios::binary);
    if(!fin){
        std::cout<<line;
        std::cout<<"\t\t!!! NO History !!!\n";
        std::cout<<line;
        return;
    }
    auto rstr=[&](std::string& s){
        std::uint64_t len=0;
        if(!fin.read(reinterpret_cast<char*>(&len),sizeof(len))) return false;
        s.resize(len);
        fin.read(&s[0],len);
        return true;
    };
    std::string action,when,details;
    int i=1;
    std::cout<<line;
    while(rstr(action) && rstr(when) && rstr(details)){
        std::cout<<i++<<".\t"<<action<<"\t"<<when<<"\t"<<details<<"\n";
    }
    if(i==1) std::cout<<"\t\t!!! NO History !!!\n";
    std::cout<<line;
    fin.close();

}
void writeOnfile( const std::vector<Student> &v){
            std::ofstream fout("records.bin",std::ios::binary|std::ios::trunc);
            if(!fout){
                std::cout<<"cannot open for write"<<"\n";
            }
            std::uint64_t n=v.size();
            fout.write(reinterpret_cast<const char*>(&n),sizeof(n));
            for(const Student& s : v){
                s.save(fout);
            }
    
}
std::vector<Student> readOnfile(){
            std::vector<Student> w;
            std::ifstream fin("records.bin",std::ios::binary);
            if(!fin) return w;
            std::uint64_t n=0;
            fin.read(reinterpret_cast<char*>(&n),sizeof(n));
            w.resize(n);
            for(Student& s : w){
                s.load(fin);
            }
            return w;
}
void Student::save(std::ostream& out) const{
    auto wstr=[&](const std::string & s){
        std::uint64_t len=s.size();
        out.write(reinterpret_cast<const char*>(&len),sizeof(len));
        out.write(s.data(),len);
    };
    out.write(reinterpret_cast<const char*>(&id),sizeof(id));
    out.write(reinterpret_cast<const char*>(&semester),sizeof(semester));
    out.write(reinterpret_cast<const char*>(&marks),sizeof(marks));
    wstr(name); wstr(dob); wstr(course);
}
void Student::load(std::istream& in){
    auto rstr=[&](std::string & s){
        std::uint64_t len=0;
        in.read(reinterpret_cast<char*>(&len),sizeof(len));
        s.resize(len);
        in.read(&s[0],len);
    };
    in.read(reinterpret_cast<char*>(&id),sizeof(id));
    in.read(reinterpret_cast<char*>(&semester),sizeof(semester));
    in.read(reinterpret_cast<char*>(&marks),sizeof(marks));
    rstr(name); rstr(dob); rstr(course);
}
void Student::setData(void){
    std::cout<<"Enter a Name::";
    std::cin>>name;
    std::cout<<"Enter id no::";
    std::cin>>id;
    std::cout<<"Enter a Dob::";
    std::cin>>dob;
    std::cout<<"Enter a course::";
    std::cin>>course;
    std::cout<<"Enter a semester::";
    std::cin>>semester;
    std::cout<<"Enter a Marks::";
    std::cin>>marks;
}
void Record::showRecords(std::vector<Student> & vec){
    int i=0;
    if(vec.empty()){
        std::cout<<"\n"<<line;
        std::cout<<"\t\t!!! NO Records ADDED in System !!!"<<"\n";
        std::cout<<line;
    }
    else{
        std::cout<<"\n";
        std::cout<<line;
        std::cout<<"\t\tThese Records Peresent in System\n";
        std::cout<<line;
        std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
        for(Student s:vec){
            std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
            i++;
        }
        std::cout<<line;
    }
}
void Record::addRecord(std::vector<Student>& vec){
    Student s1;
    bool flag=true;
    s1.setData();
     for(Student s:vec){
        if(s1.id==s.id){
            flag=false;
            break;
        }
    }
    if(flag){
        vec.push_back(s1);
        writeOnfile(vec);
        logHistory("ADD REOCRD","Id::"+std::to_string(s1.id)+" Name:"+s1.name);
        std::cout<<line;
        std::cout<<"\tGive Student Record ADDED in System Records Successfully\n";
        std::cout<<line;
    }

    else{
        std::cout<<line;
        std::cout<<"!! Student "<<s1.name<<" ID Number Already Exist in Record  !! plz change ID\n";
        std::cout<<line;
    }
   
}
void Record::deleteRecord(std::vector<Student>& vec){
    Student s1;
    std::string name;
    int id,i=0;
    char ch;
    bool flag=false;
    std::cout<<"Enter a Name of Student you wnat delete Record:";
    std::cin>>name;
    std::cout<<"Enter a Id of Studnet:";
    std::cin>>id;
    for(Student s:vec){
        if(s.name==name && s.id==id){
            s1=s;
            flag=true;

            break;
        }
        i++;
    }
    if(flag){
        std::cout<<line;
        std::cout<<"SNO\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
        std::cout<<i+1<<".\t"<<s1.id<<"\t"<<s1.name<<"\t  "<<s1.dob<<"\t"<<s1.course<<"\t\t   "<<s1.semester<<"\t\t"<<s1.marks<<std::endl;
        std::cout<<line;
        std::cout<<"!!! You Want Permament delete Record !!! y/n:\n";
        std::cin>>ch;
        if(ch=='y'||ch=='Y'){
            vec.erase(vec.begin()+i);
            writeOnfile(vec);
            logHistory("DELETE RECORD ","Id::"+std::to_string(s1.id)+"Name:"+s1.name);
            
            std::cout<<line;
            std::cout<<"\t\tStudent \""<<s1.name<<"\" Record DELETED Successully \n";
            std::cout<<line;
        }
        
    }
    else{
        std::cout<<line;
        std::cout<<"\t\t!!! There is NO Record in System !!!"<<"\n";
        std::cout<<line;
    }
    
}
void Record::updateRecord(std::vector<Student>& vec){
    char ch;
    Student s1;
    std::string name;
    int id,i=0;
    bool flag=false;
    std::cout<<"Enter a Student Name you want update Record:";
    std::cin>>name;
    std::cout<<"Enter a Id of Student";
    std::cin>>id;
    for(Student s:vec){
        if(s.name==name && s.id==id){
            s1=s;
            flag=true;
            break;
        }
        i++;
    }
    if(flag){
        std::string temp,temp2;
        int tempint,tempint2;
        float tempfloat,tempfloat2;
        std::cout<<"SNO\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
        std::cout<<i+1<<".\t"<<s1.id<<"\t"<<s1.name<<"\t  "<<s1.dob<<"\t"<<s1.course<<"\t\t   "<<s1.semester<<"\t\t"<<s1.marks<<std::endl;
        std::cout<<line;
        std::cout<<"Press 1 to Update Name\n"
        <<"Press 2 to Update Id\n"
        <<"Press 3 to Update Dob\n"
        <<"Press 4 to Update Course\n"
        <<"Press 5 to Update Semester\n"
        <<"Press 6 to Update Marks\n";
        std::cout<<line;
        std::cout<<"Enter your chioce(1-5):";
        std::cin>>ch;
        switch(ch){
            case '1':  
                std::cout<<"Enter a New Name::";
                std::cin>>temp;
                temp2=vec[i].name;
                vec[i].name=temp;
                writeOnfile(vec);
                logHistory("UPDATE","Id::"+std::to_string(s1.id)+" Name: "+temp2+" -> "+vec[i].name);
                
                std::cout<<line;
                std::cout<<"\t\tUPDATED 'Name' "<<temp2<<"->"<<vec[i].name<<" in System Record SNO "<<i+1<<"\n";
                std::cout<<line;
                break;
                case '2':
                std::cout<<"Enter a New ID::";
                std::cin>>tempint;
                tempint2=vec[i].id;
                vec[i].id=tempint;
                writeOnfile(vec);
                logHistory("UPDATE","Name "+s1.name+"id: "+std::to_string(tempint2)+" -> "+std::to_string(vec[i].id));
                std::cout<<line;
                std::cout<<"\t\tUPDATED 'Id' "<<tempint2<<"->"<<vec[i].id<<" in System Record SNO "<<i+1<<"\n";
                std::cout<<line;
                break;
                case '3':
                std::cout<<"Enter a New Dob::";
                std::cin>>temp;
                temp2=vec[i].dob;
                vec[i].dob=temp;
                writeOnfile(vec);
                logHistory("UPDATE","Id::"+std::to_string(s1.id)+" Dob: "+temp2+" -> "+vec[i].dob);
                std::cout<<line;
                std::cout<<"\t\tUPDATED 'Dob' "<<temp2<<"->"<<vec[i].dob<<" in System Record SNO "<<i+1<<"\n";
                std::cout<<line;
                break;
                
                case '4':
                std::cout<<"Enter a New Course::";
                std::cin>>temp;
                temp2=vec[i].course;
                vec[i].course=temp;
                writeOnfile(vec);
                logHistory("UPDATE","Id::"+std::to_string(s1.id)+" Course: "+temp2+" -> "+vec[i].course);
                std::cout<<line;
                std::cout<<"\t\tUPDATED 'Course' "<<temp2<<"->"<<vec[i].course<<" in System Record SNO "<<i+1<<"\n";
                std::cout<<line;
                break;
                case '5':
                std::cout<<"Enter a New Semester::";
                std::cin>>tempint;
                tempint2=vec[i].semester;
                vec[i].semester=tempint;
                writeOnfile(vec);
                logHistory("UPDATE","Id::"+std::to_string(s1.id)+" Semester: "+std::to_string(tempint2)+" -> "+std::to_string(vec[i].semester));
                std::cout<<line;
                std::cout<<"\t\tUPDATED 'Semester' "<<tempint2<<"->"<<vec[i].semester<<" in System Record SNO "<<i+1<<"\n";
                std::cout<<line;
                break;
                case '6':
                std::cout<<"Enter a New Marks::";
                std::cin>>tempfloat;
                tempfloat2=vec[i].marks;
                vec[i].marks=tempfloat;
                writeOnfile(vec);
                logHistory("UPDATE","Id::"+std::to_string(s1.id)+" Marks: "+std::to_string(tempfloat2)+" -> "+std::to_string(vec[i].marks));
                std::cout<<line;
                std::cout<<"\t\tUPDATED 'Marks' "<<tempfloat2<<"->"<<vec[i].marks<<" in System Record SNO "<<i+1<<"\n";
                std::cout<<line;
                break;
            default:
                std::cout<<line;
                std::cout<<"\t\t!! You Press Invalid option !!\n";
                std::cout<<line;


        }
    }
    else{
        std::cout<<line;
        std::cout<<"\t\t!!!  There Is NO Record You want to Update  !!!\n";
        std::cout<<line;
    }

}
void Record::searchRecord(std::vector<Student>& vec){
    Student s1;
    std::string name;
    int id,i=0;
    bool Isfound=false;
    std::cout<<"Enter a Student NAME you wnat to Search:";
    std::cin>>name;
    std::cout<<"Enter a Student Id:";
    std::cin>>id;
    for(Student s : vec ){
        if(s.name==name && s.id==id ){
            s1=s;
            Isfound=true;
            break;
        }
        i++;
    }
    if(Isfound){
        std::cout<<line;
        std::cout<<"\t\tSearching Record of Student Here on SNO. "<<i+1<<"\n";
        std::cout<<line;
        std::cout<<"SNO\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
        std::cout<<i+1<<".\t"<<s1.id<<"\t"<<s1.name<<"\t  "<<s1.dob<<"\t"<<s1.course<<"\t\t   "<<s1.semester<<"\t\t"<<s1.marks<<std::endl;

    }
    else {
        std::cout<<line;
        std::cout<<"\t  !! Is NOT found IN Records !! plz check name and id is correct ?\n";
        std::cout<<line;
    }
}

void Record::searchSimilarTypeRecord(std::vector<Student>& vec){
    int i=0;
    std::string temp,colname;
    
    std::cout<<"Enter a column to searching similar type Record:";
    std::cin>>colname;
    std::cout<<"Enter a col data to searching similar type Record:";
    std::cin>>temp;

    if(vec.empty()){
        std::cout<<"\n"<<line;
        std::cout<<"\t\t!!! NO Records in System this similar type !!!"<<"\n";
        std::cout<<line;
    }
    else{
        std::cout<<"\n";
        std::cout<<line;
        std::cout<<"\t\tThese Records Peresent in System Similar type of COLUMN \n";
        std::cout<<line;
        std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
        for(Student s:vec){
            if(colname=="dob"|| colname=="Dob" || colname=="DOB"){
                if(s.dob==temp){

                    std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                }
            }
            else if(colname=="marks"|| colname=="Marks" || colname=="MARKS"){
                if(s.marks==std::stof(temp)){

                    std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                }
            }
            else if(colname=="name"|| colname=="Name" || colname=="NAME"){
                if(s.name==temp){

                    std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                }
            }
            else if(colname=="semester"|| colname=="Semester" || colname=="SEMESTER "){
                if(s.semester==std::stoi(temp)){

                    std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                }
            }
            else if(colname=="course"|| colname=="Course" || colname=="COURSE"){
                if(s.course==temp){

                    std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                }
            }
            i++;
        }
        std::cout<<line;
    }
}
std::vector<Student> Sorting::ByName(std::vector<Student> vec,std::string order){
    if(order=="ascending"|| order=="Ascending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].name>vec[j+1].name){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    
    else if(order=="descending"|| order=="Descending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].name<vec[j+1].name){
                    swap(vec[j],vec[j+1]);
                }   
            }
        }
    }
    
    return vec;
}
std::vector<Student> Sorting::ById(std::vector<Student> vec,std::string order){
    
    if(order=="ascending"|| order=="Ascending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].id>vec[j+1].id){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    else if(order=="descending"|| order=="Descending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].id<vec[j+1].id){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    
    return vec;
}
std::vector<Student> Sorting::ByMarks(std::vector<Student> vec,std::string order){
    if(order=="ascending"|| order=="Ascending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].marks>vec[j+1].marks){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    else if(order=="descending"|| order=="Descending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].marks<vec[j+1].marks){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    return vec;
}
std::vector<Student> Sorting::BySemester(std::vector<Student> vec,std::string order){
    if(order=="ascending"|| order=="Ascending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].semester>vec[j+1].semester){
                    swap(vec[j],vec[j+1]);
                
                }
            }
        }
    }
    else if(order=="descending"|| order=="Descending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].semester<vec[j+1].semester){
                    swap(vec[j],vec[j+1]);
                
                }
            }
        }
    }
    return vec;
}
std::vector<Student> Sorting::ByCourse(std::vector<Student> vec,std::string order){
    if(order=="ascending"|| order=="Ascending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].course>vec[j+1].course){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    else if(order=="descending"|| order=="Descending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].course<vec[j+1].course){
                    swap(vec[j],vec[j+1]);
                }
            }
        }
    }
    return vec;
}
std::vector<Student> Sorting::ByDob(std::vector<Student> vec,std::string order){
    
    auto changeFormat=[](std::string s){
            return s.substr(6,4)+s.substr(2,4)+s.substr(0,2);
    };
    auto changeFormatOld=[](std::string s){
            return s.substr(8,2)+s.substr(4,4)+s.substr(0,4);
    };
    for(Student &s : vec ){
        s.dob=changeFormat(s.dob);
    }
    if(order=="ascending"|| order=="Ascending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].dob>vec[j+1].dob){
            
                    swap(vec[j],vec[j+1]);
                    
                }
            }
        }
    }
    else if(order=="descending"|| order=="Descending"){
        for(int i=0;i<vec.size()-1;i++){
            for(int j=0;j<vec.size()-1-i;j++){
                if(vec[j].dob<vec[j+1].dob){
                    std::reverse(((vec[j]).dob).begin(),((vec[j]).dob).end());
                    std::reverse(((vec[j+1]).dob).begin(),((vec[j+1]).dob).end());
                    swap(vec[j],vec[j+1]);
                    std::reverse(((vec[j]).dob).begin(),((vec[j]).dob).end());
                    std::reverse(((vec[j+1]).dob).begin(),((vec[j+1]).dob).end());
                }
            }
        }
    }
    for(Student &s : vec ){
        s.dob=changeFormatOld(s.dob);
    }
    return vec;
}

void Record::sortingRecord(std::vector<Student>& vec){
    int i=0;
    Sorting sorting;
    std::vector<Student> vec1;
    std::string temp,order;
    std::cout<<"Enter a order sorting(Ascending/Descending)::";
    std::cin>>order;
    std::cout<<"Enter a column HeadName::";
    std::cin>>temp;


    if(vec.empty()){
        std::cout<<"\n"<<line;
        std::cout<<"\t\t!!! NO Records in System to sort !!!"<<"\n";
        std::cout<<line;
    }
    else if(order=="ascending"||order=="Ascending"|| order=="descending"|| order=="Descending"){

        std::cout<<"\n";
        
        
        if(temp=="name"|| temp=="Name" || temp=="NAME"){
            std::cout<<line;
            std::cout<<"  These Records Peresent in System Sort by COLUMN '"<<temp<<" in "<<order<<"' \n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            vec1=sorting.ByName(vec,order);
            for(Student s:vec1){
                std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                i++;
            }
            std::cout<<line;
        }
        else if(temp=="id"|| temp=="Id" || temp=="ID"){
            std::cout<<line;
            std::cout<<"  These Records Peresent in System Sort by COLUMN '"<<temp<<" in "<<order<<"' \n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            vec1=sorting.ById(vec,order);
            for(Student s:vec1){
                std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                i++;
            }
            std::cout<<line;
        }
        else if(temp=="marks"|| temp=="Marks" || temp=="MARKS"){
            std::cout<<line;
            std::cout<<"  These Records Peresent in System Sort by COLUMN '"<<temp<<" in "<<order<<"' \n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            vec1=sorting.ByMarks(vec,order);
            for(Student s:vec1){
                std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                i++;
            }
            std::cout<<line;
        }
        else if(temp=="semester"|| temp=="Semester" || temp=="SEMESTER"){
            std::cout<<line;
            std::cout<<"  These Records Peresent in System Sort by COLUMN '"<<temp<<" in "<<order<<"' \n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            vec1=sorting.BySemester(vec,order);
            for(Student s:vec1){
                std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                i++;
            }
            std::cout<<line;
        }
        else if(temp=="course"|| temp=="Course" || temp=="COURSE"){
            std::cout<<line;
            std::cout<<"  These Records Peresent in System Sort by COLUMN '"<<temp<<" in "<<order<<"' \n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            vec1=sorting.ByCourse(vec,order);
            for(Student s:vec1){
                std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                i++;
            }
            std::cout<<line;
        }
        else if(temp=="dob"|| temp=="Dob" || temp=="DOB"){
            std::cout<<line;
            std::cout<<"  These Records Peresent in System Sort by COLUMN '"<<temp<<" in "<<order<<"' \n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            vec1=sorting.ByDob(vec,order);
            for(Student s:vec1){
                std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;
                i++;
            }
            std::cout<<line;
        }
        else{
            std::cout<<line;
            std::cout<<"\t\t!!!  You enter Wrong headName of Column  !!!\n";
            std::cout<<line;
        }
       
    }
    else{
        std::cout<<line;
        std::cout<<"\t!!! You enter Invalid order !!! try again enter order\n";
        std::cout<<line;
    }


}
void Record::showTopperRecords(std::vector<Student> & vec){
    int i=0;
    bool isTopper=false;
    if(vec.empty()){
        std::cout<<"\n"<<line;
        std::cout<<"\t\t!!! NO Records ADDED in System  !!!"<<"\n";
        std::cout<<line;
    }
    else{
        std::cout<<"\n";
        for(Student s:vec){
            if(s.marks>=90){
                isTopper=true;
                break;
            }
        }
        if(isTopper){
            std::cout<<line;
            std::cout<<"\t\tThese Records Peresent in System (Toppers)\n";
            std::cout<<line;
            std::cout<<"SN.\tId  \tName\t  Dob  \t\tCourse  \tSemester  \tMarks\n";
            for(Student s:vec){
                if(s.marks>=90){
                    std::cout<<i+1<<".\t"<<s.id<<"\t"<<s.name<<"\t  "<<s.dob<<"\t"<<s.course<<"\t\t   "<<s.semester<<"\t\t"<<s.marks<<std::endl;

                }
                i++;

            }
        }
        else{
            std::cout<<line;
            std::cout<<"\t\tNo Records in System Toppers\n";
            std::cout<<line;
        }

        std::cout<<line;
    }
}
void welcome(void){
    std::cout<<"\n\t\t\t\tWelcom To\n";
    std::cout<<line;
    std::cout<<"\t\tStudent Records Mangament System\n";
    std::cout<<line;

}
int main(){
    Record record;
    char choice;
    std::vector<Student> vec;
    welcome();
    vec=readOnfile();
    do{
        std::cout<<"\n";
        std::cout<<"|Commands|\n";
        std::cout<<line;
        std::cout<<"Press 1 to VIEW STUDETNS Records"<<"\n"
        <<"Press 2 to ADD NEW STUDENT Record"<<"\n"
        <<"Press 3 to DELETE STUDENT Record"<<"\n"
        <<"Press 4 to UPDATE STUDENT Record"<<"\n"
        <<"Press 5 to SEARCH STUDENT Record"<<"\n"
        <<"Press 6 to SEARCH SimilarType(column) Record"<<"\n"
        <<"Press 7 to SORTING STUDENT Records"<<"\n"
        <<"Press 8 to SHOW TOPPER Records"<<"\n"
        <<"Press H to SHOW HISTORY"<<"\n"
        <<"Press C to CLEAR HISTORY"<<"\n"
        <<"Press 9 to EXIT\n";
        std::cout<<line;
        std::cout<<"Enter a chioce::";
        std::cin>>choice;
        switch(choice){
            case '1': record.showRecords(vec); break;
            case '2': record.addRecord(vec); break;
            case '3': record.deleteRecord(vec);  break;
            case '4': record.updateRecord(vec); break;
            case '5': record.searchRecord(vec);break;
            case '6': record.searchSimilarTypeRecord(vec); break;
            case '7': record.sortingRecord(vec); break;
            case '8': record.showTopperRecords(vec); break;
            case '9': exit(1); break;
            case 'H':case 'h': showHistory(); break;
            case 'C':case 'c': clearHistory(); break;
            default:
                std::cout<<line;
                std::cout<<"\t\t!!! Your Enter WRONG command  !!!\n";
                std::cout<<line;
        }
    }while(choice!=9);
    return 0;
}