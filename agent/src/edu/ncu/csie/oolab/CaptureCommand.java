package edu.ncu.csie.oolab;

import java.io.IOException;
import java.util.HashMap;

import org.sikuli.script.Screen;
import org.sikuli.script.ScreenImage;

public class CaptureCommand extends AbstractCommand {

	public CaptureCommand( Screen screen ) {
		this.screen_ = screen;
	}

	@Override
	public HashMap< String, Object > execute( HashMap< String, Object > args ) {
		HashMap< String, Object > result = new HashMap< String, Object >();
		ScreenImage img = this.screen_.userCapture();
		if( img == null ) {
			result.put( "success", false );
			result.put( "message", "empty area" );
			return result;
		}
		String path = null;
		try {
			path = img.getFilename();
		} catch( IOException e ) {
			result.put( "success", false );
			result.put( "message", "picture path error" );
			return result;
		}
		result.put( "success", true );
		result.put( "path", path );
		return result;
	}

	private Screen screen_;

}
