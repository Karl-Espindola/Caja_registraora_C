#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>


typedef struct{
    int anio;
    int mes;
    int dia;
    int hora;
    int min;
    int seg;
}Fecha;

typedef struct{
    int id;
    int idProducto;
    int cantidad;
    float precioTotal;

}Comprado;

typedef struct{
    int id;
    Fecha tiempo;
    char comprador[80];
    Comprado compra[100];
    int cantidadDeProductos;    //Numero de productos diferentes agregados a la compra
    float total;
}Ventas;

typedef struct{
    int id;
    char nombre[80];
    int precio;
    int cantidad;

}Productos;

int anio;
int mes;
int dia;
int hora;
int min;
int seg;

const char rutaProductos[80] = "productos.bin";
const char rutaVentas[80] = "ventas.bin";
const char rutaTemporal[80] = "temporal.bin";

int crearProducto();
int ponerEnElcarrito();
Productos consultarProducto(int idProducto);
int listarProductos();
void editarProducto(int idProducto);
void eliminarProducto(int idProducto);
Productos restarStock(int idProducto, int cantidad);
int verificarProuctosRepetidos(char nomProducto[80]);

void tiempoActual();
void factura(int idVenta);
void listarTodasLasVentas();

void menuInventario();

int main(){

    int opc;
    
    while (opc != 0)
    {
        int existeProducto;
        int idVenta;
        tiempoActual();

        FILE *fp = fopen(rutaProductos, "rb");
        if(!feof(fp)){
            existeProducto = 1;
        }
        else{
            existeProducto = -1;
        }
        fclose(fp);

        system("cls");
        printf("\n******* MENU PRINCIPAL *******\n");
        printf("\n1: Registrar venta");
        printf("\n2: Inventario");
        printf("\n3: Listar ventas");
        printf("\n0: Salir");
        printf("\n\nOpcion: ");
        scanf("%i", &opc);

        switch (opc)
        {
        case 1:
            if(existeProducto == 1){

                idVenta = ponerEnElcarrito();
                factura(idVenta);
            }
            else if(existeProducto == -1){
                system("cls");
                printf("\nNo hay productos. Registre algun producto\n");
            }
            getch();
            break;
        case 2:
            menuInventario();
            break;
        case 3:
            if(existeProducto == 1){
                listarTodasLasVentas();
            }
            else if(existeProducto == -1){
                system("cls");
                printf("\nNo hay productos. Registre algun producto\n");
            }
            getch();
            break;
        case 0:
            break;
        default:
            printf("\n\nOpcion no valida");
            getch();
        }

    }
    
    return 0;
}

void menuInventario(){
    int opc;
    while (opc != 0)
    {
        int existe;
        int idProducto;

        system("cls");
        printf("\n******* Menu inventario *******\n");
        printf("\n1: Crear producto");
        printf("\n2: Listar productos");
        printf("\n3: Editar");
        printf("\n4: Borrar");
        printf("\n0: regresar");
        printf("\n\nOpcion: ");
        scanf("%i", &opc);

        switch (opc)
        {
        case 1:
            crearProducto();
            getch();
            break;
        case 2:
            system("cls");
            existe = listarProductos();
            if(existe != 1){
                printf("\nNo hay productos\n");
            }
            getch();
            break;
        case 3:
            system("cls");
            listarProductos();
            printf("\n\nIngrese el ID del producto: ");
            scanf("%i", &idProducto);
            editarProducto(idProducto);
            getch();
            break;
        case 4:
            system("cls");
            listarProductos();
            printf("\n\nIngrese el ID del producto: ");
            scanf("%i", &idProducto);
            eliminarProducto(idProducto);
            getch();
            break;
        case 0:
            break;
        default:
            printf("\n\nOpcion no valida");
        }

    }
}

void tiempoActual(){ // Obtenemos la hora y fecha actuales //
    
    time_t actual = time(NULL); 
    struct tm *tm = localtime(&actual);
    anio= tm->tm_year+1900;
    mes = tm->tm_mon+1;
    dia = tm->tm_mday;
    hora = tm->tm_hour;
    min = tm->tm_min;
    seg = tm->tm_sec;
}

// ##########################################################

int crearProducto(){

    Productos producto;
    char nombreProducto[80];
    int id = 0;
    int existe;
    FILE *fproductos = fopen(rutaProductos, "rb+");

    /* ID autoincrementable para Productos */

    fread(&producto, sizeof(Productos), 1, fproductos);
    while (!feof(fproductos))
    {
        id = producto.id;
        fread(&producto, sizeof(Productos), 1, fproductos);
    }
    if(id != 0){
        producto.id = id + 1;
        /* Fin de autoincrementable para Productos */

        system("cls");
        fflush(stdin);
        printf("\nIngresa el nombre del producto: ");
        gets(nombreProducto);
        existe = verificarProuctosRepetidos(nombreProducto);
        if(existe == 1){
            printf("\nEste producto ya existe");
            return 0;
        }
        printf("\nIngresa el precio del producto: ");
        scanf("%i", &producto.precio);
        printf("\nIngresa el cantidad del producto: ");
        scanf("%i", &producto.cantidad);
        strcpy(producto.nombre, nombreProducto);

        if(producto.precio >= 0 && producto.cantidad >= 0){
            fwrite(&producto, sizeof(Productos), 1, fproductos);
            printf("\nProducto Registrado\n");
        }
        else{
            printf("\nNo se admiten valores negativos. Registro cancelado\n");
        }
    }
    else{
        producto.id = 1;
        system("cls");
        fflush(stdin);
        printf("\nIngresa el nombre del producto: ");
        gets(nombreProducto);
        existe = verificarProuctosRepetidos(nombreProducto);
        if(existe == 1){
            printf("\nEste producto ya existe");
            getch();
            return 0;
        }
        printf("\nIngresa el precio del producto: ");
        scanf("%i", &producto.precio);
        printf("\nIngresa el cantidad del producto: ");
        scanf("%i", &producto.cantidad);
        strcpy(producto.nombre, nombreProducto);

        if(producto.precio >= 0 && producto.cantidad >= 0){
            fwrite(&producto, sizeof(Productos), 1, fproductos);
            printf("\nProducto Registrado\n");
        }
        else{
            printf("\nNo se admiten valores negativos. Registro cancelado\n");
        }
    }
    fclose(fproductos);
    return 0;
}

int listarProductos(){
   
    Productos producto;
    int existe;
   
    FILE *fproductos = fopen(rutaProductos, "rb");
    fread(&producto, sizeof(Productos), 1, fproductos);
    while (!feof(fproductos))
    {
        existe = 1;
        printf("\nID: %i  Producto: %s  Precio: $%i  Cantidad: %i", producto.id, producto.nombre, producto.precio, producto.cantidad);
        fread(&producto, sizeof(Productos), 1, fproductos);
    }

    fclose(fproductos);
    return existe;
}

Productos consultarProducto(int idProducto){
    Productos producto;
    FILE * fp;
    fp = fopen(rutaProductos, "rb");
    fread(&producto, sizeof(Productos), 1, fp);
    while(!feof(fp)){
        if(producto.id == idProducto){
            fclose(fp);
            return producto;
        }
        fread(&producto, sizeof(Productos), 1, fp);
    }
    producto.id = -1;
    fclose(fp);
    return producto;
}

void editarProducto(int idProducto){
    Productos producto;
    char nuevoNombre[80];
    int nuevaCantidad;
    float nuevoPrecio;
    int existe;
    FILE *fp = fopen(rutaProductos, "rb+");

    fread(&producto, sizeof(Productos), 1, fp);

    while(!feof(fp)){
        if(producto.id == idProducto){
            fflush(stdin);
            printf("\nIngrese el nuevo titulo: \n");
            fgets(nuevoNombre, 79, stdin);

            if(strcmp(nuevoNombre, "\n") != 0){ //Si se presiona Enter, entonces deja el nombre originar sin modificar

                strcpy(producto.nombre, nuevoNombre);
            }
            
            printf("Ingrese la nueva cantidad: \n");
            scanf("%i", &nuevaCantidad);
            printf("Ingrese el nuevo precio: \n");
            scanf("%f", &nuevoPrecio);
            if(nuevaCantidad > 0 && nuevoPrecio >= 0){
                    
                producto.cantidad = nuevaCantidad;
                producto.precio = nuevoPrecio;
                fseek(fp, -(sizeof(Productos)), SEEK_CUR);
                fwrite(&producto, sizeof(Productos), 1, fp);
                printf("\nEl producto fue editado\n");
            }
            else{
                printf("\nCantidad o Precio menor a 0. No se realizo ningun cambio");
            }
            existe = 1;
            break;
        }
        fread(&producto, sizeof(Productos), 1, fp);
    }
    if(existe != 1){
        printf("\nEl producto no existe\n");
    }
    fclose(fp);
}

void eliminarProducto(int idProducto){

    Productos producto;
    int existe;

    FILE *fp = fopen(rutaProductos, "rb");
    FILE *ft = fopen(rutaTemporal, "wb");

    fread(&producto, sizeof(Productos), 1, fp);

    while (!feof(fp))
    {
        if(producto.id == idProducto){
            existe = 1;
        }
        if(producto.id != idProducto){
            fwrite(&producto, sizeof(Productos), 1, ft);
        }
        fread(&producto, sizeof(Productos), 1, fp);
    }
    if(existe != 1){
        printf("\nEL producto no existe\n");
    }
    else{
        printf("\nProducto eliminado\n\n");
    }
    fclose(fp);
    fclose(ft);
    remove(rutaProductos);
    rename(rutaTemporal, rutaProductos);
    
    
}

Productos restarStock(int idProducto, int cantidad){ // Cada vez que se registra la venta de un producto, se resta la cantidad comprada del inventario 
    Productos producto;

    FILE *fp = fopen(rutaProductos, "rb+");
    fread(&producto, sizeof(Productos), 1, fp);
    while (!feof(fp)){
        if(producto.id == idProducto){
            producto.cantidad = producto.cantidad - cantidad;
            fseek(fp, -sizeof(Productos), SEEK_CUR);
            fwrite(&producto, sizeof(Productos), 1, fp);
            break;
        }
        fread(&producto, sizeof(Productos), 1, fp);
    }
    fclose(fp);
    return producto;
}

int verificarProuctosRepetidos(char nomProducto[80]){
    Productos producto;

    FILE *fp = fopen(rutaProductos, "rb+");
    fread(&producto, sizeof(Productos), 1, fp);
    while (!feof(fp))
    {
        if(strcmp(nomProducto, producto.nombre) == 0){
            fclose(fp);
            return 1;
        }
        fread(&producto, sizeof(Productos), 1, fp);
    }
    fclose(fp);
    return 0;
}

// ################################################################

int ponerEnElcarrito(){
    
    Ventas venta;
    Productos producto;
    int opc1;
    int id = 0;
    char nombreComprador[80];
    FILE *fv;
    fv = fopen(rutaVentas, "rb+");

    /* ID autoincrementable para las ventas */
    fread(&venta, sizeof(Ventas), 1, fv);
    while(!feof(fv)){
        id = venta.id;
        fread(&venta, sizeof(Ventas), 1, fv);
    }
    if(id == 0){
        venta.id = 1;
    }
    /* Fin autoincrementable para las ventas */

    /* Asignación de la fecha a la venta */
    venta.id = id + 1;
    venta.tiempo.anio = anio;
    venta.tiempo.mes = mes;
    venta.tiempo.dia= dia;
    venta.tiempo.hora = hora;
    venta.tiempo.min = min;
    venta.tiempo.seg = seg;

    system("cls");
    fflush(stdin);
    printf("\nIngrese el nombre del comprador: ");
    gets(nombreComprador);
    strcpy(venta.comprador, nombreComprador);
    int i = 0;
    int existe;
    int cantidad = 0;
    int acumulador;

    while(opc1 != 2){
        
        system("cls");
        existe = listarProductos();
        if(existe != 1){
            printf("\nNo hay productos\n");
            fclose(fv);
            return -1;
        }
        else if(existe == 1){
            printf("\n\n----- Productos en el Carrito -----\n");
            for (int x = 0; x < i; x++){
                
                producto = consultarProducto(venta.compra[x].idProducto);
                printf("\nProducto: %s cantidad: %i total: $%.2f", 
                producto.nombre, venta.compra[x].cantidad, venta.compra[x].precioTotal);
            }
            printf("\n-------------------");
            printf("\n1: Elegir producto");
            printf("\n2: Finalizar compra");
            printf("\nOpcion: ");
            scanf("%i", &opc1);
            
            if(opc1 == 1){

                system("cls");
                listarProductos();
                printf("\n\nIngrese el ID del producto: ");
                scanf("%i", &venta.compra[i].idProducto);
                producto = consultarProducto(venta.compra[i].idProducto);

                if(producto.id != -1 ){ //Producto existe
                    if(producto.cantidad <= 0){
                        system("cls");
                        printf("\nNo hay existencias disponibles");
                        getch();
                        continue;
                    }
                }
                else if(producto.id == -1){
                    system("cls");
                    printf("\nEste producto No existe");
                    getch();
                    continue;
                }

                
                int cantidadStock = producto.cantidad;
                do{
                    int facturado;
                    cantidadStock = producto.cantidad;
                    for(int j = 0; j < i; j++){
                        if(venta.compra[j].idProducto == producto.id){
                            
                            facturado = 1;
                            printf("\nIngrese la cantidad: ");
                            scanf("%i", &acumulador);
                            
                            if(acumulador > producto.cantidad){
                                printf("\nNo tenemos esa cantidad. Puede llevar maximo %i unidades\n", producto.cantidad);
                            }
                            else{
                                producto = restarStock(producto.id, acumulador);
                                cantidad = venta.compra[j].cantidad;
                                venta.compra[j].cantidad = (cantidad + acumulador);
                                venta.compra[j].precioTotal = (producto.precio) * (venta.compra[j].cantidad);
                            } 
                        }

                    }

                    if(facturado != 1){
                        acumulador = 0;
                        cantidad = 0;
                        
                        printf("\nIngrese la cantidad: ");
                        scanf("%i", &acumulador);
                        venta.compra[i].cantidad = cantidad + acumulador;
                        cantidad = venta.compra[i].cantidad;
                        if(acumulador > producto.cantidad){
                            printf("\nNo tenemos esa cantidad. Puede llevar maximo %i unidades\n", producto.cantidad);
                        }
                        else{
                     
                            producto = restarStock(producto.id, acumulador);
                            venta.compra[i].precioTotal = (producto.precio) * (venta.compra[i].cantidad);
 
                        }
                    }
                    facturado = 0;
                }
                while(acumulador > cantidadStock );

                venta.compra[i].precioTotal = (producto.precio) * (venta.compra[i].cantidad);
                
                int z = i;
                int repetido = 0;

                for (int x = 0; x < z; x++){
                    if(venta.compra[i].idProducto == venta.compra[x].idProducto ){
                        repetido = 1;
                        cantidad = 0;
                        acumulador = 0;
                        
                    }
                }
                
                if(repetido != 1){
                    
                    i = i + 1;
                }
                
            }
            else if(opc1 == 2){
                break;
            } 
        }
    }
    float suma = 0;
    for(int x = 0; x < i; x++){
        suma = suma + venta.compra[x].precioTotal;
    }
    venta.total = suma; // Precio total por todos los productos comprados
    venta.cantidadDeProductos = i; //Numero de productos registrados en la compra. Este numero se usa despues para recorrer la lista de compras
    int id_venta = venta.id;
    fwrite(&venta, sizeof(Ventas), 1, fv);
    fclose(fv);
    
    return id_venta;
    
}

void listarTodasLasVentas(){
    Ventas venta;
    int i = 1;
    FILE *fv;
    fv = fopen(rutaVentas, "rb+");
    
    fread(&venta, sizeof(Ventas), 1, fv);
    if(feof(fv)){
        system("cls");
        printf("\nNo hay ninguna\n");
    }
    system("cls");
    while(!feof(fv)){
        factura(i);
        printf("\n");
        i++;
        fread(&venta, sizeof(Ventas), 1, fv);
    }
    fclose(fv);
}

void factura(int idVenta){ //Imprime la factura 
    Ventas venta;
    Productos producto;

    FILE *fv;
    fv = fopen(rutaVentas, "rb");

    fread(&venta, sizeof(Ventas), 1, fv);
    while(!feof(fv)){
        if(venta.id == idVenta){
            break;
        }
        fread(&venta, sizeof(Ventas), 1, fv);
    }

    printf("\n------------------ FACTURA ----------------\n");
    printf("\nID: %i  Nombre: %s Fecha %.2d/%.2d/%.2d  %.2d:%.2d:%.2d\n", venta.id, venta.comprador, 
    venta.tiempo.dia, venta.tiempo.mes, venta.tiempo.anio, venta.tiempo.hora, venta.tiempo.min, venta.tiempo.seg);
    
    int i = venta.cantidadDeProductos;
    for (int x = 0; x < i; x++){
        
        producto = consultarProducto(venta.compra[x].idProducto);
        printf("\nProducto: %s cantidad: %i total: $%.2f", 
        producto.nombre, venta.compra[x].cantidad, venta.compra[x].precioTotal);
    }
    printf("\nTOTAL:                             $%.2f\n", venta.total);
    printf("--------------------------------------------");

}





















