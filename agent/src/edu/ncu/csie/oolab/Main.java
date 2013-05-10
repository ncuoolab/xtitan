package edu.ncu.csie.oolab;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;

public class Main {

	/**
	 * @param args
	 */
	public static void main( String[] args ) {
		for( Integer i = 0; i < args.length; ++i ) {
			if( args[i].equals( "--quite" ) ) {
				CommandParser.setVerbose( false );
			}
		}
		// disables Dock icon on Apple system
		System.setProperty( "apple.awt.UIElement", "true" );

		InputStreamReader reader = null;
		reader = new InputStreamReader( System.in, Charset.forName( "UTF-8" ) );
		BufferedReader cin = new BufferedReader( reader );

		CommandParser parser = new CommandParser();

		System.out.println( "ready" );

		while( true ) {
			String line = null;
			try {
				line = cin.readLine();
			} catch( IOException e ) {
				e.printStackTrace();
				break;
			}
			if( line == null ) {
				// EOF
				break;
			}
			line = parser.execute( line.trim() );
			if( line.equals( "bnVsbA==" ) ) {
				// null object in base64
				break;
			}
			try {
				// NOTE dirty hack
				// if end tag is sent too soon, the QProcess may not emit readRead 
				Thread.sleep( 500 );
			} catch( InterruptedException e ) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println( line );
		}
	}

}
