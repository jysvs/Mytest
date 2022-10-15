package ugr.pabloescobedo.poc_nfc;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.nfc.tech.NfcA;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

import static ugr.pabloescobedo.poc_nfc.MainActivity.memory;
import static ugr.pabloescobedo.poc_nfc.MainActivity.file_Name;

/**
 * Created by pabloescobedo@ugr.es
 * Class to establish communication through NFC link
 */
public class NfcComm implements Runnable {

    private static final String TAG = "NfcComm";
    // Operating modes (Read and Write)
    private static final byte READ_MODE = 0x00;
    private static final byte WRITE_MODE = 0x01;
    private static final byte FILE_MODE = 0x02;
    // Type of messages (only in write mode)
    private static final byte START_MESSAGE = 0x00;
    private static final byte STOP_MESSAGE = 0x01;

    private static final byte addrWrite = 0x05; // Dirección donde se escribe la condición de inicio de medida
    private static final byte addrRead = 0x04; // Dirección donde se escribe la condición de fin de medida

    // Member data from NfcComm class
    NfcA mNfcA;
    byte mode;
    byte message;
    byte[] signalCodification = new byte[]{0, 0, 0, 0};

    public NfcComm(NfcA mNfcA,byte mode, byte message) {
        this.mNfcA = mNfcA;
        this.mode = mode;
        this.message = message;
    }

    public NfcComm(NfcA mNfcA,byte mode, byte message, byte[] signalCodification) {
        this.mNfcA = mNfcA;
        this.mode = mode;
        this.message = message;
        this.signalCodification = signalCodification;
    }

    public void run(){
        Log.v(TAG, "Thread run");
        switch(mode){
            case READ_MODE:
                // Read from EEPROM
                createFile();
                readEE();
                break;

            case WRITE_MODE:
                switch (message){
                    // Write to EEPROM with start condition
                    case START_MESSAGE:
                        Log.v(TAG, "Start Thread");
                        startMeasurement();
                        break;
                    // Write to EEPROM with stop condition
                    case STOP_MESSAGE:
                        Log.v(TAG, "Stop Thread");
                        stopMeasurement();
                        break;
                    default:
                        break;
                }
                break;

            case FILE_MODE:
                break;

            default:
                break;
        }

    }


    /*
    * Function to write on the EEPROM of the NFC chip
    * @param byte addr address to write
    * @param byte[] dataWrite vector
     */
    public void writeEE(byte addr, byte[] dataWrite){

        //Comprobar que está conectada la tag
        if (mNfcA.isConnected() == true) {
            Log.d(TAG, "writeEE method: still connected");
            byte[] resp = null;

            /* Comando para escribir
             * 1º byte : -94  (write mode)
             * 2º byte : address
             * 3º byte : data
             */
            byte[] cmd = new byte[]{(byte) -94, addr, dataWrite[3], dataWrite[2], dataWrite[1], dataWrite[0]};

            try {
                //We send the command to AS3955
                resp = mNfcA.transceive(cmd);
                //Check the response. Send again if not received
                while (resp == null || resp.length != 16) {
                    resp = mNfcA.transceive(cmd);
                }

            } catch (IOException tagLostEx) {}
        }else{
            Log.d(TAG, "writeEE method: disconnected");
            Log.e(TAG,"Tag disconnected");
        }
    }


    //Function to read fata from EEPROM of AS3955
    public void readEE(){
        // Check if connected
        if (mNfcA.isConnected() == true) {
            //Get the positions from the EEPROM memory (dataUser block)
            for (int i=addrRead; i<121; i=i+4) {
                byte[] resp = null;
                /* Command to read
                 * 1º byte : 48  (read mode)
                 * 2º byte : address
                 */
                byte[] cmd = new byte[]{(byte) 48, (byte) i};
                try {
                    //We send the command to AS3955
                    resp = mNfcA.transceive(cmd);

                    //Check the response. Send again if not received
                    while (resp == null || resp.length != 16) {
                        resp = mNfcA.transceive(cmd);
                        //if (isCancelled()){
                        //    break;
                        //}
                    }

                    byte cont = 0;

                    //Save the data and convert to String with HEX format
                    for (int j = 0; j < resp.length; j = j + 4) {

                        memory[cont+i-4][0] = String.format("%02X", resp[j] & 0xFF);
                        memory[cont+i-4][1] = String.format("%02X", resp[j + 1] & 0xFF);
                        memory[cont+i-4][2] = String.format("%02X", resp[j + 2] & 0xFF);
                        memory[cont+i-4][3] = String.format("%02X", resp[j + 3] & 0xFF);
                        cont++;

                        try {

                            File sdCard = Environment.getExternalStorageDirectory();
                            File myFile = new File(sdCard.getAbsolutePath() + "/ECLDetector" + "/" + file_Name);
                            if (!myFile.exists()) {
                                myFile.createNewFile();
                                Log.i(TAG, "File created");
                            }

                            FileOutputStream fOut = new FileOutputStream(myFile.getAbsoluteFile(), true);
                            OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);

                            //write to CSV
                            myOutWriter.write(String.format("%02X", resp[j] & 0xFF)+String.format("%02X\n", resp[j + 1] & 0xFF)+String.format("%02X", resp[j + 2] & 0xFF)+String.format("%02X\n", resp[j + 3] & 0xFF));
                            myOutWriter.flush();

                        } catch (Exception e) {
                            e.printStackTrace();
                        }


                    }
                } catch (IOException tagLostEx) {

                }
            }
        }else{
        }
    }

    public void startMeasurement(){
        byte[] measurementCode = new byte[]{1, 0, 0, 0};
        signalCodification[0] = 1;
        writeEE(addrWrite,signalCodification);
    }

    public void stopMeasurement(){
        byte[] measurementCode = new byte[]{0, 0, 0, 0};
        signalCodification[0] = 0;

        writeEE(addrWrite,signalCodification);
    }

    void createFile() {
        try {
            SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMdd_HHmmss");
            Date now = new Date();
            file_Name = "Data_" + formatter.format(now) + ".csv";

            File dir = new File("/mnt/sdcard/SmartFacemask/");
            if (!dir.exists()) {
                dir.mkdir();
                Log.i(TAG, "Directory created");
            }

            File sdCard = Environment.getExternalStorageDirectory();
            File myFile = new File(sdCard.getAbsolutePath() + "/SmartFacemask" + "/" + file_Name);
            if (!myFile.exists()) {
                myFile.createNewFile();
                Log.i(TAG, "File created");
            }

            FileOutputStream fOut = new FileOutputStream(myFile.getAbsoluteFile(), true);
            OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);

            //Write the header
            myOutWriter.write(formatter.format(now)+"_data\n");
            myOutWriter.flush();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
