#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip> // Necesario para setw y setfill
#include<cstdlib>
#include<thread>   
#include<chrono>
#include<conio.h>

using namespace std;


struct registro{
	int codigo;
	string nombre;
	int costo;
	int cantidad;
	bool inicializado=false;
	
	registro(int i, const string& n, int p, int x, bool a) 
    : codigo(i), nombre(n), costo(p), cantidad(x), inicializado(a) {}

	registro() : codigo(0), nombre(""), costo(0), cantidad(0), inicializado(false) {}
};

struct usuarios{
	string usuario;
	string contrasena;
	int autoridad;
	bool inicializado=false;
	
	usuarios(const string& n,const string& p,int i,bool a) 
    : usuario(n), contrasena(p),autoridad(i),inicializado(a) {}

	usuarios() : usuario(string("")),contrasena(string("")),autoridad(0), inicializado(false){}
};

vector<registro> productos;
vector<usuarios> users;

void GuardarU(const vector<usuarios>& arr, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error al abrir el archivo para guardar." << endl;
        return;
    }

    for (const auto& users : arr) {
        if (users.inicializado) {
            outFile << users.usuario << " "
                    << users.contrasena << " "
					<< users.autoridad << " "
                    << users.inicializado << endl;
        }
    }

    outFile.close();
    if (!outFile) {
        cerr << "Error al cerrar el archivo al guardar." << endl;
    }
}

int login(vector<usuarios>& arr) {
    int intentos = 0,control,i=0;
    bool condicion = true, romper=false;
    string usuario, contrasena;

	cout<<"--------------------------"<<endl;
	cout<<"Bienvenido al Programa de Inventario"<<endl;
	cout<<"--------------------------"<<endl;
    while (condicion) {
		cout<<"Inicia Sesion o registrate:"<<endl;
		cout<<"1)Iniciar Sesion"<<endl;
		cout<<"2)Registrarse"<<endl;
		control=getch();

		switch(control){
			case '1':{
				while(romper==false){
					cout<< "Introduce tu usuario: ";
					cin >> usuario;

					cout<< "Introduce tu contrasena: ";
					cin >> contrasena;
					for (size_t i = 0; i < users.size(); ++i) {
						if (usuario == users[i].usuario && contrasena == users[i].contrasena) {

							cout <<"Bienvenido, "<<usuario<<"! Has iniciado sesion con exito."<<endl;
							this_thread::sleep_for(chrono::seconds(2));
							system("cls"); 
							return users[i].autoridad; 

						}
					}
						
					if(intentos<2){
						cout<<"Error: credenciales incorrectas. Intenta de nuevo."<<endl;
						cout<<endl;
					}
					else{
						cout<<"Has superado el limite de intentos. El programa se cerrara."<<endl;
						return -1;
					}
					intentos++;
				}
				break;	
			}
			case '2':{
				usuarios user_nuevo;
				
				cout<<"Ingrese su nombre de usuario:"<<endl;
				cin>>user_nuevo.usuario;
				cout<<"Ingrese su contrasena:"<<endl;
				cin>>user_nuevo.contrasena;
				user_nuevo.autoridad=1;
				user_nuevo.inicializado=true;
				users.push_back(user_nuevo);
				GuardarU(users, "RegistroUsuarios.txt");
				break;	
			}
			default:{
				cout<<"Opcion Invalida, Por favor seleccione una opcion valida."<<endl;
				cout<<endl;
			}
		}
        
    }
    return 0; 
}

void Ordenar(vector<registro>& arr) {//BubbleSort
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compara elementos adyacentes y los intercambia si están en el orden incorrecto
            if (arr[j].codigo > arr[j + 1].codigo) {
                // Intercambio utilizando una variable temporal
                registro temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


int busquedaBin(vector<registro>& arr, int clave){
    int central, bajo, alto;
    int valorCentral;
    bajo = 0;
    alto = arr.size()-1;
    while (bajo <= alto){
		central = (bajo + alto)/2; 
		valorCentral = arr[central].codigo; 
		if (clave == valorCentral){
			return central; 
		}
		else if (clave < valorCentral){
			alto = central -1; 
		}
		else{
			bajo = central + 1; 
		}
    }
    return -1; 
}

bool CompararDuplicado(const vector<registro>& arr, const string& campo, const string& valor) {
    for (const auto& producto : arr) {
        if (campo == "codigo" && to_string(producto.codigo) == valor) {
            return true;
        } else if (campo == "nombre" && producto.nombre == valor) {
            return true;
        }
    }
    return false;
}

void Cargar(vector<registro>& arr, const string& filename) {
	ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error al abrir el archivo para cargar." << endl;
        return;
    }

    arr.clear();

    string linea;
    getline(inFile, linea); // Saltar encabezado CSV

    while (getline(inFile, linea)) {
        stringstream ss(linea);
        string valor;

        int codigo, costo, cantidad;
        string nombre;
        bool inicializado;

        getline(ss, valor, ',');
        try {
            codigo = stoi(valor);
        } catch (const std::invalid_argument& e) {
            cerr << "Error de conversión de 'codigo': " << valor << endl;
            continue;
        }

        getline(ss, valor, '"');
        getline(ss, nombre, '"'); 
        getline(ss, valor, ','); 

        getline(ss, valor, ',');
        try {
            costo = stoi(valor);
        } catch (const std::invalid_argument& e) {
            cerr << "Error de conversión de 'costo': " << valor << endl;
            continue;
        }

        getline(ss, valor, ',');
        try {
            cantidad = stoi(valor);
        } catch (const std::invalid_argument& e) {
            cerr << "Error de conversión de 'cantidad': " << valor << endl;
            continue;
        }

        getline(ss, valor, ',');
        try {
            inicializado = stoi(valor);
        } catch (const std::invalid_argument& e) {
            cerr << "Error de conversión de 'inicializado': " << valor << endl;
            continue;
        }

        arr.push_back(registro(codigo, nombre, costo, cantidad, inicializado));
    }

    inFile.close();
}

void CargarU(vector<usuarios>& arr, const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error al abrir el archivo para cargar." << endl;
        return ;
    }

    arr.clear(); 

    string usuario,contrasena;
	int autoridad;
    bool inicializado;

    while (inFile >> usuario >> contrasena >> autoridad >> inicializado) {
        arr.push_back(usuarios(usuario, contrasena, autoridad, inicializado));
    }

    inFile.close();
}

void Guardar(const vector<registro>& arr, const string& filename) {
	
     ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error al abrir el archivo para guardar." << endl;
        return;
    }

    outFile << "codigo,nombre,costo,cantidad,inicializado" << endl;

    for (const auto& producto : arr) {
        if (producto.inicializado) {
            outFile << producto.codigo << ",\""
                    << producto.nombre << "\","
                    << producto.costo << ","
                    << producto.cantidad << ","
                    << producto.inicializado << endl;
        }
    }

    outFile.close();
    if (!outFile) {
        cerr << "Error al cerrar el archivo al guardar." << endl;
    }
	
}
    
int main(){

	Cargar(productos, "RegistroProductos.csv");
	CargarU(users,"RegistroUsuarios.txt");
	
    int intentos = 0;
	char opcion;
	int i=0;
	int cod_prod;
	string codigo;
	int posicion;
	bool activo=true;
	
	int autoridad=login(users);

	if(autoridad!= 1 && autoridad!= 2){
		return 1;
	}

	while(activo){
		cout<<"--------------------------"<<endl;
		cout<<"SISTEMA DE REGISTRO DE PRODUCTOS"<<endl;
		cout<<"--------------------------"<<endl;
		cout<<"SELECCIONE LA OPCION:"<<endl;
		cout<<"1.INSERTAR"<<endl;
		cout<<"2.BUSCAR"<<endl;
		cout<<"3.ACTUALIZAR"<<endl;
		cout<<"4.ELIMINAR"<<endl;
		cout<<"5.MOSTRAR PRODUCTOS"<<endl;
		cout<<"6.SALIR"<<endl;
		cout<<"Ingrese la opcion:"<<endl;
		opcion=getch();
		cout<<""<<endl;

		if(autoridad==2){
			switch(opcion){
				case '1':{

					cout << "Opcion 1 seleccionada: INSERTAR" << endl;

					registro producto_nuevo;
					string valor;

					cout<<"Ingrese el codigo del producto que quiere ingresar:"<<endl;
					cin >> valor;
					while (CompararDuplicado(productos, "codigo", valor)) {
						cout << "Error: el codigo ya existe. Ingrese un codigo diferente: ";
						cin >> valor;
					}
                	producto_nuevo.codigo = stoi(valor);
					cout<<"Ingrese el nombre del producto que quiere ingresar:"<<endl;
					cin >> valor;
					while (CompararDuplicado(productos, "nombre", valor)) {
						cout << "Error: el nombre ya existe. Ingrese un nombre diferente: ";
						cin >> valor;
					}
					producto_nuevo.nombre = valor;
					cout<<"Ingrese el costo del producto que quiere ingresar:"<<endl;
					cin>>producto_nuevo.costo;
					cout<<"Ingrese la cantidad del producto que quiere ingresar:"<<endl;
					cin>>producto_nuevo.cantidad;
					producto_nuevo.inicializado=true;
					productos.push_back(producto_nuevo);
					Ordenar(productos);
					cout<<"Producto agregado correctamente."<<endl;
					cout<<""<<endl;
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					break;	
				}
				case '2': {
					cout << "Opcion 2 seleccionada: BUSCAR" << endl;
					
					bool continuar = true;
					while (continuar) {
						cout << "Ingrese el codigo del producto que desea buscar:" << endl;
						cin >> cod_prod;
						posicion = busquedaBin(productos, cod_prod);

						char opcion;
						bool contrl=true;
						if (posicion != -1) {
							cout<<"----------------------------------"<<endl;
							cout<<"Codigo   Nombre   Costo   Cantidad"<<endl;
							cout<<"----------------------------------"<<endl;
							cout<<setw(3)<<setfill(' ')<<productos[posicion].codigo<<"    "
								<<left<<productos[posicion].nombre<<"   "
								<<productos[posicion].costo<<"      "
								<<productos[posicion].cantidad<<endl;
							cout<< "----------------------------------"<<endl;

						
							cout<<"¿Desea buscar otro producto?(s/n): "<<endl;
							
							while(contrl==true){
								opcion=getch();
								if (opcion == 's' || opcion == 'S') {
									continuar = true; 
									contrl=false;
								}
								else if(opcion == 'n' || opcion == 'N'){
									continuar=false;
									contrl=false;
									
								}
								else{
									cout<<"Seleccion no valida.Intente de nuevo."<<endl;
								}	
							}	
						} 
						else {
							cout<<"Producto no encontrado."<<endl;
							cout<<"¿Desea buscar otro producto?(s/n): "<<endl;

							while(contrl==true){
								opcion=getch();
								if (opcion == 's' || opcion == 'S') {
									continuar = true; 
									contrl=false;
								}
								else if(opcion == 'n' || opcion == 'N'){
									continuar=false;
									contrl=false;
								
								}
								else{
									cout<<"Seleccion no valida.Intente de nuevo."<<endl;
								}	
							}	
						}
					}
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					break;
				}
				case '3':{
					char opcion;
					bool contrl=true;
					bool continuar = true;
					cout <<"Opcion 3 seleccionada: ACTUALIZAR" << endl;
					cout<<"Ingrese el codigo del producto que desea actualizar:"<<endl;
					cin>>cod_prod;
					posicion=busquedaBin(productos,cod_prod);
					
					while (continuar) {
						if (posicion != -1){
							
							string valor;
							cout<<"Seleccione que desea actualizar:"<<endl;
							cout<<"1.CODIGO"<<endl;
							cout<<"2.NOMBRE"<<endl;
							cout<<"3.COSTO"<<endl;
							cout<<"4.CANTIDAD"<<endl;
							opcion=getch();
						
							switch (opcion){
								
								 case '1':{
									cout<<"Ingrese el nuevo codigo:"<<endl;
									cin>>valor;
									while(CompararDuplicado(productos,"codigo",valor)){
										cout<<"Error: el codigo ya existe. Ingrese un codigo diferente:"<<endl;
										cin>>valor;
									}
									productos[posicion].codigo=stoi(valor);
									cout<<"Producto actualizado correctamente:"<<endl;
									break;
								}
								case '2':{
									cout<<"Ingrese el nuevo nombre:"<<endl;
									cin>>valor;
									while(CompararDuplicado(productos,"nombre",valor)){
										cout<<"Error: el nombre ya existe. Ingrese un nombre diferente:"<<endl;
										cin>>valor;
									}
									productos[posicion].nombre=valor;
									cout<<"Producto actualizado correctamente:"<<endl;
									break;
								}
								case '3':{
									cout<<"Ingrese el nuevo costo:"<<endl;
									cin>>(productos[posicion].costo);
									cout << "Producto actualizado correctamente:" << endl;
									break;
								}
								case '4':{
									cout<<"Ingrese la nueva cantidad:"<<endl;
									cin>>(productos[posicion].cantidad);
									cout << "Producto actualizado correctamente:" << endl;
		
									break;
								}
								case '5':{
									break;
								}
							}
						}
						else {
							
							cout<<"Producto no encontrado."<<endl;
							cout<<"¿Desea buscar otro producto?(s/n): "<<endl;

							while(contrl==true){
								opcion=getch();
								if (opcion == 's' || opcion == 'S') {
									continuar = true; 
									contrl=false;
								}
								else if(opcion == 'n' || opcion == 'N'){
									continuar=false;
									contrl=false;
								
								}
								else{
									cout<<"Seleccion no valida.Intente de nuevo."<<endl;
								}	
							}	
						}
					}
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					break;
				}
				case '4':{
					bool continuar = true;
					bool contrl=true; 
					cout <<"Opcion 4 seleccionada: ELIMINAR"<<endl;
					
					while (continuar) { 
				
						cout<<"Ingrese el codigo del producto que desea eliminar:"<<endl;
						cin>>cod_prod;
						posicion=busquedaBin(productos,cod_prod);
						
						if (posicion != -1) {
							productos.erase(productos.begin() + posicion);
							cout<<"Producto elminado corretamete."<<endl;
							continuar=false;
						}
						else{
							cout<<"Producto no encontrado."<<endl;
							cout<<"¿Desea buscar otro producto?(s/n): "<<endl;
	
							while(contrl==true){
								opcion=getch();
								if (opcion == 's' || opcion == 'S') {
									continuar = true; 
									contrl=false;
								}
								else if(opcion == 'n' || opcion == 'N'){
									continuar=false;
									contrl=false;
								}
								else{
									cout<<"Seleccion no valida.Intente de nuevo."<<endl;
								}
							}
						}
					}
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					break;
				}
				case '5':{
					
					bool contrl=true;
					cout <<"Opcion 5 seleccionada: MOSTRAR PRODUCTOS" << endl;
					int tamano=productos.size();
					cout<<"----------------------------------"<<endl;
					cout<<"Codigo   Nombre   Costo   Cantidad"<<endl;
					cout<<"----------------------------------"<<endl;
					
					for (int i = 0; i < tamano; i++){
					
			       		 cout << setw(3) << setfill(' ') << productos[i].codigo << "    "
			             << left << setw(10) << productos[i].nombre << "   "
			             << setw(6) << productos[i].costo << "      "
			             << productos[i].cantidad << endl;
			    	
						cout<< "----------------------------------"<<endl;		
					}
					
					cout<<"Volver al menu(s): "<<endl;
							
					while(contrl==true){
						opcion=getch();
						if (opcion == 's' || opcion == 'S') {
							contrl=false;
							this_thread::sleep_for(chrono::seconds(1));
							system("cls");
						}
						else{
							cout<<"Seleccion no valida.Intente de nuevo."<<endl;
						}	
					}
						
					break;
				}
				case '6':{
					cout <<"Opcion 6 seleccionada: SALIR"<<endl;
					cout <<"Saliendo..."<<endl;
					Guardar(productos, "RegistroProductos.csv");
					cout << "Datos guardados correctamente." << endl;
					return 0;
				}
				default:{
					cout << "\nOpcion invalida" << endl;
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
					break;	
				}
			}
		}
		else{
			switch (opcion){
				case '2': {
					cout << "Opcion 2 seleccionada: BUSCAR" << endl;
					
					bool continuar = true;
					while (continuar) {
						cout << "Ingrese el codigo del producto que desea buscar:" << endl;
						cin >> cod_prod;
						posicion = busquedaBin(productos, cod_prod);

						char opcion;
						bool contrl=true;
						if (posicion != -1) {
							cout<<"----------------------------------"<<endl;
							cout<<"Codigo   Nombre   Costo   Cantidad"<<endl;
							cout<<"----------------------------------"<<endl;
							cout<<setw(3)<<setfill('0')<<productos[posicion].codigo<<"    "
								<<left<<productos[posicion].nombre<<"   "
								<<productos[posicion].costo<<"      "
								<<productos[posicion].cantidad<<endl;
							cout<< "----------------------------------"<<endl;

						
							cout<<"¿Desea buscar otro producto?(s/n): "<<endl;
							
							while(contrl==true){
								opcion=getch();
								if (opcion == 's' || opcion == 'S') {
									continuar = true; 
									contrl=false;
								}
								else if(opcion == 'n' || opcion == 'N'){
									continuar=false;
									contrl=false;
									this_thread::sleep_for(chrono::seconds(1));
									system("cls");
								}
								else{
									cout<<"Seleccion no valida.Intente de nuevo."<<endl;
								}	
							}	
						} 
						else {
							cout<<"Producto no encontrado."<<endl;
							cout<<"¿Desea buscar otro producto?(s/n): "<<endl;

							while(contrl==true){
								opcion=getch();
								if (opcion == 's' || opcion == 'S') {
									continuar = true; 
									contrl=false;
								}
								else if(opcion == 'n' || opcion == 'N'){
									continuar=false;
									contrl=false;
									this_thread::sleep_for(chrono::seconds(1));
									system("cls");
								}
								else{
									cout<<"Seleccion no valida.Intente de nuevo."<<endl;
								}	
							}	
						}
					}

					break;
				}
				case '5':{
					
					bool contrl=true;
					cout <<"Opcion 5 seleccionada: MOSTRAR PRODUCTOS" << endl;
					int tamano=productos.size();
					cout<<"----------------------------------"<<endl;
					cout<<"Codigo   Nombre   Costo   Cantidad"<<endl;
					cout<<"----------------------------------"<<endl;
					
					for (int i = 0; i < tamano; i++){
					
			       		 cout << setw(3) << setfill(' ') << productos[i].codigo << "    "
			             << left << setw(10) << productos[i].nombre << "   "
			             << setw(6) << productos[i].costo << "      "
			             << productos[i].cantidad << endl;
			    	
						cout<< "----------------------------------"<<endl;		
					}
					
					cout<<"Volver al menu(s): "<<endl;
							
					while(contrl==true){
						opcion=getch();
						if (opcion == 's' || opcion == 'S') {
							contrl=false;
							this_thread::sleep_for(chrono::seconds(1));
							system("cls");
						}
						else{
							cout<<"Seleccion no valida.Intente de nuevo."<<endl;
						}	
					}
						
					break;
				}
				case '6':{
					cout<<""<<endl;
					cout<<"Opcion 6 seleccionada: SALIR"<<endl;
					cout<<"Saliendo..."<<endl;
					Guardar(productos, "RegistroProductos.csv");
					cout<<"Datos guardados correctamente."<<endl;
					return 0;
				}
				default:
					cout<<"No cuentas con los suficientes permisos para utilizar esta opcion."<<endl;
					this_thread::sleep_for(chrono::seconds(1));
					system("cls");
				break;
			}
		}
	}
	
	system("PAUSE");
    return 0;
}
