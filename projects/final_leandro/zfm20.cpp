
#include "zfm20.hpp"

Zfm20::Zfm20(void)
{
    // Resets data members
    this->_packageData      = nullptr;
    this->_packageIndex     = 0;
    this->_packageLength    = 0;
    this->_packageMaxSize   = 0;
    this->_initialized      = false;
    this->_deviceAddress    = 0xFFFFFFFF;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

bool_t Zfm20::init(cuint8_t bufferSize_p)
{
    // Checks for errors
    if(bufferSize_p < 12) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_SMALL;
        return false;
    }

    // Deallocates previously allocated memory
    if(isPointerValid(this->_packageData)) {
        free(this->_packageData);
        this->_packageData = nullptr;
    }

    // Allocates memory
    this->_packageData = (uint8_t *)calloc(bufferSize_p, sizeof(uint8_t));
    if(!isPointerValid(this->_packageData)) {
        // Returns error
        this->_lastError = Error::MEMORY_ALLOCATION;
        return false;
    }

    // Updates data members
    this->_initialized      = true;
    this->_packageMaxSize   = bufferSize_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Zfm20::setAddress(cuint32_t deviceAddress_p)
{
    // TODO: Checks if there are any forbidden addresses

    // Updates data members
    this->_deviceAddress = deviceAddress_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Zfm20::createPackage(Id packageId_p, uint8_t *packageData_p, uint8_t packageSize_p)
{
    uint16_t auxChecksum = 0;

    // Checks for errors
    if(!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(!isPointerValid(packageData_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        return false;
    }
    if(packageSize_p == 0) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_ZERO;
        return false;
    }
    if(packageSize_p > (this->_packageMaxSize - 11)) {
        // Returns error
        this->_lastError = Error::BUFFER_SIZE_TOO_LARGE;
        return false;
    }

    // Creates package
    this->_packageIndex = 0;
    this->_packageLength = 0;
    auxChecksum = 0;
    // Header (2 bytes)
    this->_packageData[this->_packageIndex++] = 0xEF;
    this->_packageData[this->_packageIndex++] = 0x01;
    // Address (4 bytes)
    this->_packageData[this->_packageIndex++] = wordGetByte3(this->_deviceAddress);
    this->_packageData[this->_packageIndex++] = wordGetByte2(this->_deviceAddress);
    this->_packageData[this->_packageIndex++] = wordGetByte1(this->_deviceAddress);
    this->_packageData[this->_packageIndex++] = wordGetByte0(this->_deviceAddress);
    // Id (1 byte)
    this->_packageData[this->_packageIndex++] = (uint8_t)packageId_p;
    auxChecksum += (uint16_t)packageId_p;
    // Length (2 bytes)
    this->_packageData[this->_packageIndex++] = hwordGetByte1(packageSize_p + 2);
    auxChecksum += (uint16_t)hwordGetByte1(packageSize_p + 2);
    this->_packageData[this->_packageIndex++] = hwordGetByte0(packageSize_p + 2);
    auxChecksum += (uint16_t)hwordGetByte0(packageSize_p + 2);
    // Data ('packageSize_p' bytes)
    for(uint8_t i = 0; i < packageSize_p; i++) {
        this->_packageData[this->_packageIndex++] = packageData_p[i];
        auxChecksum += (uint16_t)packageData_p[i];
    }
    // Checksum (2 bytes)
    this->_packageData[this->_packageIndex++] = hwordGetByte1(auxChecksum);
    this->_packageData[this->_packageIndex++] = hwordGetByte0(auxChecksum);

    // Updates data members
    this->_packageLength = this->_packageIndex;

    //FULL PACKAGE


    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

void Zfm20::debug(void)
{
    printf("Size: %u\r\n", this->_packageLength);
    printf("Data: ");
    for(uint8_t i = 0; i < this->_packageLength; i++) {
        printf("%02X ", this->_packageData[i]);
    }
    printf("\r\n\r\n");
}


uint8_t Zfm20::receive_pkg(Usart0* usart)
{
    //sendstring("entrou receivepkg \n");
    uint16_t byte= 0xFF;
    uint8_t data[12] = {255,255,255,255,255,255,255,255,255,255,255,255};
    //int i=11;
    uint8_t response;
    uint8_t i =0;
    uint8_t ii=40;
    char c[16];
    bool_t go = 1;
    while (go = 1)
    {
        //sendstring("entrou no while");
        usart->receiveData(&byte);
        sendstring("saiu da leitura");
        //sendstring("passou da leitura");
        if (i==8) //Checking for instruction length
        {
            ii=byte;
        }
        if(i==9) // getting return value
        {
            response=byte;
        }
        if (ii=0)
        {
            go = 0;
        }
        i++;
        ii--;
        // sendstring("i =");
        // sprintf(c,"%02X",i);
        // sendstring(c);
        // sendstring("byte = ");
        sprintf(c,"%02X",byte);
        sendstring(c);
    }
    return response;


    // for (int i = 12; i < 0; i--)
    // //TODO, make it able to read variable size
    // {
    //     usart->receiveData(&byte);
    //     data[i] = (uint8_t)byte;
    //     switch (i)
    //     {
    //     case 11:
    //         sendstring("case 11 ");
    //         v8bit = byte;
    //         if (v8bit == (uint8_t)SByteValues::BYTE1 ){
    //         this->_lastError=Error::FRAME_ERROR;
    //         sendstring("Primeiro byte não corresponde ao endereço ");
    //         }
    //         break;
    //     default:
    //         sendstring("default do switch case");
    //         break;
    //     }
    // }
    // sprintf(c,"%02X",data);
    // //sendstring(c);
    // return data[12];
}

bool_t Zfm20::sendPackage(Usart0* usart){
    //printf("%",this->_packageData);
    for (int i = 0; i < this->_packageLength; i++)
    {
        usart->sendData(this->_packageData[i]);
    }
    return 1;
}

 bool_t Zfm20::checkFingerPrint (Usart0* usart){
    //makes package with instruction
    uint8_t auxString[]={0x1};
    uint8_t buffer =33;
    char c[8];
    this->createPackage(Zfm20::Id::COMMAND, auxString, 1);

    //sends the package
    this->sendPackage(usart);

    //gets response

    buffer=this->receive_pkg(usart);

    //process response

    switch (buffer)
    {
    case (uint8_t)Zfm20::Rcv_Status::NO_FINGER :
        sendstring("Nenhum dedo no sensor ");
        break;
    case (uint8_t)Zfm20::Rcv_Status::IMG_ENTRY_FAILURE :
        sendstring("Imagem ruim ");
        break;
    case (uint8_t)Zfm20::Rcv_Status::MUST_CHECK_PWD :
        sendstring("Inserir senha do sensor ");
        break;
    case (uint8_t)Zfm20::Rcv_Status::NO_MATCH:
        sendstring("Dedo nao encontrado. ");
        break;
    case (uint8_t)Zfm20::Rcv_Status::OK :
        sendstring("Dedo OK! ");
        break;
    default:
        sendstring("buffer = ");
        sprintf(c,"%02X",buffer);
        sendstring(c);
        sendstring("N foi possivel verificar dedo por motivo qualquer.");
        break;
    }
    return 1;
 }
