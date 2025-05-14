package org.cocos2dx.cpp_empty_test;


import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

//this code is copied from krkrz
public class MainActivityDevelop extends Activity {
    private boolean mSelectedStartFolder;
    private boolean mOpenStartFolder;

    static final int SELECT_TREE_REQUEST_CODE = 2;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mSelectedStartFolder = false;
        mOpenStartFolder = false;

        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        if(bundle != null) {
            String path = bundle.getString("startup_path");
            if( path != null && !path.isEmpty()) {
                //nativeSetStartupPath(path);
            }
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if( mSelectedStartFolder == false ) {
            mSelectedStartFolder = true;
            mOpenStartFolder = true;
            selectFolder();
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent resultData) {
        if( mOpenStartFolder && requestCode == SELECT_TREE_REQUEST_CODE ) {
            if( resultCode == Activity.RESULT_OK ) {
                Uri treeUri = resultData.getData();
                getContentResolver().takePersistableUriPermission(treeUri, Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
                if( mOpenStartFolder ) {
                    //nativeSetStartupPath( treeUri.toString() + "/" );
                }
            }
            mOpenStartFolder = false;
        } else {
            super.onActivityResult( requestCode, resultCode, resultData );
        }
    }
    
    public void selectFolder() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
        startActivityForResult(intent, SELECT_TREE_REQUEST_CODE);
    }
}
