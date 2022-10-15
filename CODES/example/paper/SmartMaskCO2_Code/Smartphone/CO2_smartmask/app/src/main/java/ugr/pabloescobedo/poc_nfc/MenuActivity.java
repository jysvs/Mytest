package ugr.pabloescobedo.poc_nfc;

import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Button;
import android.widget.TextView;
/**
 * Created by pabloescobedo
 */

public class MenuActivity extends AppCompatActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Get the view from activity_menu.xml
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_menu);
        getSupportActionBar().setDisplayShowHomeEnabled(true);
        getSupportActionBar().setIcon(R.mipmap.mask_icon);
    }

    public void ecl(View view) {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }

    @Override
    public boolean onCreateOptionsMenu(final Menu menu) {
        getMenuInflater().inflate(R.menu.about_menu, menu);
        for (int i = 0; i < menu.size(); i++) {
            menu.getItem(i).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(final MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_about:
                showAbout();
                return true;
            case R.id.menu_share:
                share();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
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

}
