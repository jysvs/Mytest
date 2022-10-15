package ugr.pabloescobedo.poc_nfc;

import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.NfcA;
import android.os.Handler;
import android.os.SystemClock;
import android.provider.Settings;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.Arrays;

/**
 * Created by pabloescobedo@ugr.es
 */
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    // NFC adapter identifier
    NfcAdapter mNfcAdapter;
    NfcA mNfcA;

    //Array to save the data of the AS3955 EEPROM
    static String[][] memory;
    byte[] signalCodification = new byte[]{0, 0, 0, 0};

    // Identifier for NFC working operation mode (0x00 read mode, 0x01 write mode)
    private byte mNfcMode;
    private static final byte READ_MODE = 0x00;
    private static final byte WRITE_MODE = 0x01;
    // Type of messages (only in write mode)
    private static final byte START_MESSAGE = 0x00;
    private static final byte STOP_MESSAGE = 0x01;

    static boolean mTag;

    NfcComm mNfcCommStartThread, mNfcCommStopThread;

    Button readButton;

    TextView m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33, conc, tagID;
    ImageView goodBox, moderateBox, poorBox, unhealthyBox;
    double temp = 0.0;
    int Rcoord, Gcoord, Bcoord;

    Chronometer simpleChronometer;

    volatile public String fileName;
    static volatile public String file_Name;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG,"Start of onCreate");
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_memory);

        getSupportActionBar().setDisplayShowHomeEnabled(true);
        getSupportActionBar().setIcon(R.mipmap.mask_icon);

        Log.d(TAG, "Check if phone has NFC adapter");

        mNfcAdapter = NfcAdapter.getDefaultAdapter(this);
        if(mNfcAdapter==null){
            Toast.makeText(this,"This device does not support NFC technology", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

        Log.d(TAG,"Check if NFC is active");
        if(!mNfcAdapter.isEnabled()){
            Toast.makeText(this,"Please turn on NFC",Toast.LENGTH_SHORT).show();
            // Start activity to NFC settings in phone
            startActivity(new Intent(Settings.ACTION_NFC_SETTINGS));
        }
        // Defualt operation mode: read mode
        mNfcMode = READ_MODE;
        mNfcCommStopThread = new NfcComm(mNfcA,WRITE_MODE,STOP_MESSAGE);

        memory = new String[120][4];
        m00 = (TextView) findViewById(R.id.m00);
        m01 = (TextView) findViewById(R.id.m01);
        m02 = (TextView) findViewById(R.id.m02);
        m03 = (TextView) findViewById(R.id.m03);
        m10 = (TextView) findViewById(R.id.m10);
        m11 = (TextView) findViewById(R.id.m11);
        m12 = (TextView) findViewById(R.id.m12);
        m13 = (TextView) findViewById(R.id.m13);
        m20 = (TextView) findViewById(R.id.m20);
        m21 = (TextView) findViewById(R.id.m21);
        m22 = (TextView) findViewById(R.id.m22);
        m23 = (TextView) findViewById(R.id.m23);
        m30 = (TextView) findViewById(R.id.m30);
        m31 = (TextView) findViewById(R.id.m31);
        m32 = (TextView) findViewById(R.id.m32);
        m33 = (TextView) findViewById(R.id.m33);
        conc = (TextView) findViewById(R.id.conc);
        tagID = (TextView) findViewById(R.id.tagId);

        goodBox = (ImageView) findViewById(R.id.imgView_good);
        moderateBox = (ImageView) findViewById(R.id.imgView_moderate);
        poorBox = (ImageView) findViewById(R.id.imgView_poor);
        unhealthyBox = (ImageView) findViewById(R.id.imgView_unhealthy);

        readButton = (Button) findViewById(R.id.readButton);

        simpleChronometer = (Chronometer) findViewById(R.id.simpleChronometer); // initiate a chronometer

        Log.d(TAG,"End of onCreate");
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater mMenuInflater = getMenuInflater();
        mMenuInflater.inflate(R.menu.main_menu, menu);
        return true;
    }

    // Pablo
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item.getGroupId());

        switch (item.getGroupId()) {
            default:
                switch (item.getItemId()) {

                    case R.id.action_about:
                        // User chose the "Settings" item, show the app settings UI...
                        Log.d(TAG, "About_selection");
                        showAbout();
                        return true;

                    case R.id.menu_share:
                        share();
                        return true;

                    default:
                        // If we got here, the user's action was not recognized.
                        // Invoke the superclass to handle it.
                        return super.onOptionsItemSelected(item);
                }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        // Check if result from activity is "RESULT_CANCELED".
        if (resultCode == RESULT_CANCELED) {
            // In that case, we show this message on screen
            Toast.makeText(this, "Result cancelled", Toast.LENGTH_SHORT)
                    .show();
        } else {

            // Otherwise, we take the result from the second activity
            this.signalCodification = data.getExtras().getByteArray("signalCodification");
            Log.d(TAG,"OnActivityResult obtained "+Arrays.toString(this.signalCodification));
        }
    }

    // Function to keep an intent alive waiting for NFC tag detection
    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        Log.d(TAG,"Executing onNewIntent");
        // We check if the created intent is due to an NFC tag
        if(intent.hasExtra(mNfcAdapter.EXTRA_TAG)){
            Log.d(TAG,"EXTRA_TAG Intent detected");
            Tag tag = intent.getParcelableExtra(mNfcAdapter.EXTRA_TAG);

            // We save the tag as NfcA since we are using direct commands to stablish communication
            // with the EEPROM of the NFC chip
            mNfcA = NfcA.get(tag);
            Log.d(TAG,"NfcA object obtained");

            try{
                mNfcA.connect();
                Log.d(TAG,"Connecting to the tag");
            } catch(IOException e){
            }

            // We update the variable to say that the NFC tag was detected
            mTag = true;
            if(mNfcA.isConnected()){
                Log.d(TAG,"Reading process");
                Toast.makeText(this,"Connected to the TAG",Toast.LENGTH_SHORT).show();
                //Toast.makeText(this,"tag ID = " + bytesToHexString(tag.getId()),Toast.LENGTH_SHORT).show();
                tagID.setText(bytesToHexString(tag.getId()));

            }else{
                Log.d(TAG, "Tag disconnected");
            }
        }

    }

    private String bytesToHexString(byte[] src) {
        StringBuilder stringBuilder = new StringBuilder("");
        if (src == null || src.length <= 0) {
            return null;
        }

        char[] buffer = new char[2];
        for (int i = 0; i < src.length; i++) {
            buffer[0] = Character.forDigit((src[i] >>> 4) & 0x0F, 16);
            buffer[1] = Character.forDigit(src[i] & 0x0F, 16);
            System.out.println(buffer);
            stringBuilder.append(buffer);
        }

        return stringBuilder.toString();
    }

    protected void showAbout() {
        // Inflate the about message contents
        View messageView = getLayoutInflater().inflate(R.layout.popup_about, null, false);

        // When linking text, force to always use default color. This works
        // around a pressed color state bug.
        TextView textView = (TextView) messageView.findViewById(R.id.about_credits);
        int defaultColor = textView.getTextColors().getDefaultColor();
        textView.setTextColor(defaultColor);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        //builder.setIcon(R.drawable.app_icon);
        builder.setTitle(R.string.app_name);
        builder.setView(messageView);
        builder.create();
        builder.show();
    }

    private void share() {
        Intent intentShareFile = new Intent(android.content.Intent.ACTION_SEND);
        intentShareFile.setType("text/plain");
        intentShareFile.setAction(Intent.ACTION_SEND);
        //intentShareFile.putExtra(Intent.EXTRA_STREAM, Uri.parse("file://" + reportPath));
        startActivity(Intent.createChooser(intentShareFile, "Share Smart Facemask Results with..."));
    }

    private void enableForegroundDispatchSystem() {
        Intent intent = new Intent(this, MainActivity.class).addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, 0);
        IntentFilter[] intentFilters = new IntentFilter[]{};
        mNfcAdapter.enableForegroundDispatch(this, pendingIntent, intentFilters, null);
    }

    private void disableForegroundDispatchSystem() {
        mNfcAdapter.disableForegroundDispatch(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        enableForegroundDispatchSystem();
    }

    @Override
    protected void onPause() {
        super.onPause();
        disableForegroundDispatchSystem();
    }

    public void onDestroy() {
        super.onDestroy();
    }

    // Function when user clicks Get Value button.
    // We get the values from the EEPROM of the NFC chio and start the timer
    public void clickReadButton(View view) {
        Log.d(TAG, "Click on Read");

        //Check if there is a connected tag
        if (mNfcA.isConnected()) {
            Log.d(TAG, "Tag connected");
            //In that case, we create a thread to read the data of the NFC tag
            mNfcCommStartThread = new NfcComm(mNfcA,READ_MODE,START_MESSAGE,signalCodification);
            Thread t = new Thread(mNfcCommStartThread);
            t.start();
            try { t.join(); } catch (InterruptedException e) { e.printStackTrace(); } // wait for thread to finish

            Toast.makeText(this,"Read completed",Toast.LENGTH_SHORT).show();
            simpleChronometer.start(); // start a chronometer
            Toast.makeText(this,"Timer has been started",Toast.LENGTH_SHORT).show();

            // Show on screen the read values
            drawTable();

        } else {
            Log.d(TAG, "Tag disconnected");
        }
    }

    //Function to shown on screen the values read from the EEPROM of the NFC chip (R,G,B and Temperature)
    public void drawTable() {
        String dec_value = hex_2_dec(memory[1][2],memory[1][3]);
        m00.setText("[05h]");
        m01.setText(memory[1][2]);
        m02.setText(memory[1][3]);
        m03.setText(dec_value);
        Rcoord = Integer.parseInt(dec_value);
        dec_value = hex_2_dec(memory[2][2],memory[2][3]);
        m10.setText("[06h]");
        m11.setText(memory[2][2]);
        m12.setText(memory[2][3]);
        m13.setText(dec_value);
        Gcoord = Integer.parseInt(dec_value);
        dec_value = hex_2_dec(memory[3][2],memory[3][3]);
        m20.setText("[07h]");
        m21.setText(memory[3][2]);
        m22.setText(memory[3][3]);
        m23.setText(dec_value);
        Bcoord = Integer.parseInt(dec_value);
        dec_value = hex_2_dec(memory[4][2],memory[4][3]);
        m30.setText("[08h]");
        m31.setText(memory[4][2]);
        m32.setText(memory[4][3]);
        m33.setText(dec_2_temp(dec_value));

        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                // Temperature drift correction
                double correctedR = tempDriftCorrection(Rcoord, temp);
                // CO2 calculation using corrected R coordinate and calibration curve
                double CO2conc = getCO2(correctedR);
                // Show calculated CO2 concentration on screen
                conc.setText(String.valueOf(CO2conc));

                // Traffic light indicator
                if (CO2conc <= 1000) {
                    goodBox.setImageResource(R.drawable.box_green);
                    moderateBox.setImageResource(R.drawable.box_gray);
                    poorBox.setImageResource(R.drawable.box_gray);
                    unhealthyBox.setImageResource(R.drawable.box_gray);
                }
                else if(CO2conc > 1000 && CO2conc <= 10000) {
                    goodBox.setImageResource(R.drawable.box_gray);
                    moderateBox.setImageResource(R.drawable.box_yellow);
                    poorBox.setImageResource(R.drawable.box_gray);
                    unhealthyBox.setImageResource(R.drawable.box_gray);
                }
                else if(CO2conc > 10000 && CO2conc <= 30000) {
                    goodBox.setImageResource(R.drawable.box_gray);
                    moderateBox.setImageResource(R.drawable.box_gray);
                    poorBox.setImageResource(R.drawable.box_orange);
                    unhealthyBox.setImageResource(R.drawable.box_gray);
                }
                else { //CO2conc > 50000
                    goodBox.setImageResource(R.drawable.box_gray);
                    moderateBox.setImageResource(R.drawable.box_gray);
                    poorBox.setImageResource(R.drawable.box_gray);
                    unhealthyBox.setImageResource(R.drawable.box_red);
                }
                // Elapsed time calculation
                long elapsedtime = SystemClock.elapsedRealtime() - simpleChronometer.getBase();
                int h   = (int)(elapsedtime /3600000);
                int m = (int)(elapsedtime - h*3600000)/60000;
                int s= (int)(elapsedtime - h*3600000- m*60000)/1000 ;
                String hh = h < 10 ? "0"+h: h+"";
                String mm = m < 10 ? "0"+m: m+"";
                String ss = s < 10 ? "0"+s: s+"";
                AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                // Alert dialog showing elapsed time and CO2 concentration to give the opportunity to ventilate and reset timer
                builder.setTitle("Alert")
                        .setMessage("Elapsed time since last measurement: "+hh+":"+mm+":"+ss+"\n\nCO2 concentration is"+CO2conc+"\n\nVentilate and reset timer?")
                        .setCancelable(false)
                        .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                simpleChronometer.setBase(SystemClock.elapsedRealtime());
                                Toast.makeText(MainActivity.this,"Timer has been reset",Toast.LENGTH_SHORT).show();
                            }
                        })
                        .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                Toast.makeText(MainActivity.this,"No, I will ventilate later",Toast.LENGTH_SHORT).show();
                            }
                        });
            }
        }, 100);


    }

    // Temperature drift correction
    double tempDriftCorrection(Integer Rvalue, double temp) {
        double corrected_R = 0;
        double a = -0.0156;
        double b = 1.2861;
        double Rnorm = a*temp+b;
        corrected_R = Rnorm*Rvalue;
        return corrected_R;
    }

    // CO2 computation using obtained calibration curve
    double getCO2(double correctedR) {
        double CO2ppm = 0;
        double a = 176093;
        double b = -154898;
        double c = 27500;
        double Rmax = 6100;
        double Rnorm = correctedR/Rmax;
        CO2ppm = a*Rnorm*Rnorm + b*Rnorm + c;
        return CO2ppm;
    }

    // Hex to Dec
    public String hex_2_dec(String mem1,String mem2){
        String decimal="-";
        String leido = mem1+mem2;
        decimal = String.valueOf(Integer.parseInt(leido,16));
        return decimal;
    }

    // Temperature calculation
    public String dec_2_temp(String mem1){
        temp=(3.3*100*Integer.parseInt(mem1)/1023)-50;
        return String.format("%.2f ºC",temp);
    }

}
