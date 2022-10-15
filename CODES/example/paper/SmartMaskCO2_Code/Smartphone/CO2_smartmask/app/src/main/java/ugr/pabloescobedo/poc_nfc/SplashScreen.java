package ugr.pabloescobedo.poc_nfc;

import android.content.Intent;
import android.nfc.NfcAdapter;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class SplashScreen extends AppCompatActivity {

    private static final String TAG = "SplashScreen";

    // Here we can define for how long the spash screen will be shown
    private static int SPLASH_TIME = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_splash_screen);

        new Handler().postDelayed(new Runnable(){
            /*
            * Mostramos la pantalla de bienvenida con un temporizador.
            * Así, se puede mostrar el logo de la app durante unos segundos.
             */
            @Override
            public void run() {
                Intent i = new Intent(SplashScreen.this, MenuActivity.class);
                startActivity(i);
                finish();
            }
        }, SPLASH_TIME);

    }
}
